/*
 * Runtime.cpp
 *
 *  Created on: 2014��11��12��
 *      Author: Administrator
 */

#include <config/Config.h>
#include <thread/ThreadPool.h>
#include <concurrent/Concurrent.h>
#include <log/Log.h>
#include <utils/time/Tick.h>
#include <utils/time/TimerManager.h>

#include <concurrent/rpc/RPCManager.h>

#include "Runtime.h"

namespace zertcore{ namespace core{
using namespace zertcore::concurrent;
using namespace zertcore::time_utils;
}}

namespace zertcore{ namespace core{

bool Runtime::
globalInit(const handler_type& init_handler) {
	if (is_running_)
		return false;

	ZCLOG_SETUP();
	ZC_ASSERT(tick_type::init());

	is_running_ = true;
	init_handler_ = init_handler;

	if (!ThreadPool::Instance().
			registerExclusiveHandler(bind(&Runtime::mainThread, this),
					bind(&Runtime::startRun, this)))
		return false;

	ThreadPool::Instance().setAfterAllInitedHandler(bind(&Runtime::afterAllInited, this));
	return /*Concurrent::Instance().globalInit() &&*/
		   ThreadPool::Instance().globalInit() &&
		   initExtence();
}

bool Runtime::
initExtence() {
	return true;
}

void Runtime::
afterInitedHandler(const handler_type& handler) {
	after_inited_handler_list_.push_back(handler);
}

bool Runtime::
setupRouter(const rpc::RPCRouterConfig& config) {
	router_config_ = config;
	return true;
}

u32 Runtime::
update() {
	ZC_ASSERT_RUN_IN_SAME_THREAD();

	now_.getTime();

	u32 size = TimerManager::Instance().update();

	for (updater_list_type::iterator it = updater_list_.begin();
			it != updater_list_.end(); ) {
		updater_list_type::iterator hit = it; ++it;
		if (time_type(hit->first) > now_) {
			break;
		}

		(hit->second)();
		size++;

		updater_list_.erase(hit);
	}

	for (persistent_updater_list_type::iterator it = persistent_updater_list_.begin();
			it != persistent_updater_list_.end(); ) {
		persistent_updater_list_type::iterator hit = it; ++it;
		size += (*hit)();
	}

	return size;
}

size_t Runtime::
mainThread() {
	return update();
}

void Runtime::
afterAllInited() {
	for_each(after_inited_handler_list_.begin(), after_inited_handler_list_.end(), [] (handler_type& handler) {
		handler();
	});

	after_inited_handler_list_.clear();

	if (router_config_) {
		if (!rpc::RPCManager::Instance().connectRouter(router_config_)) {
			ZCLOG(ERROR) << "Connect to Router[" << router_config_.toString() << "] failed" << End;
		}

		rpc::RPCManager::Instance().finishRegister();
	}
}

void Runtime::
startRun() {
	if (init_handler_)
		init_handler_();

	for (auto it = init_handler_list_.begin(); it != init_handler_list_.end(); ++it) {
		ZC_ASSERT( (*it)() );
	}

	init_handler_list_.clear();
}

void Runtime::
run() {
	ThreadPool::Instance().start();
	ThreadPool::Instance().joinAll();
}

void Runtime::
stop() {
	ThreadPool::Instance().stop();
}

}}
