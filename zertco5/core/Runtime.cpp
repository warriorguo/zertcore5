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
#include <utils/time/CPUTime.h>

#include "Runtime.h"

namespace zertcore{ namespace core{
using namespace zertcore::concurrent;
}}

namespace zertcore{ namespace core{

bool Runtime::
init(const handler_type& init_handler) {
	if (is_running_)
		return false;

	ZCLOG_SETUP();
	ZC_ASSERT(time::CPUTime::init());

	is_running_ = true;
	init_handler_ = init_handler;

	if (!ThreadPool::Instance().
			registerExclusiveHandler(bind(&Runtime::mainThread, this), true))
		return false;

	return Concurrent::Instance().init() &&
			ThreadPool::Instance().init(config.concurrent.thread_nums);
}

Runtime::updater_key_type Runtime::
setTimeout(time_type interval, handler_type handler) {
	return updater_list_.insert(updater_list_type::value_type(interval.value,
			handler));
}

void Runtime::
removeTimeout(const updater_key_type& key) {
	;
}

u32 Runtime::
update() {
	ZC_ASSERT_RUN_IN_SAME_THREAD();

	now_.getTime();

	u32 size = 0;
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

void Runtime::
mainThread() {
	startRun();

	while(is_running_) {
		if (!RT.update() && !ThreadPool::Instance().runOnce(false))
			usleep(0);
	}
}

void Runtime::
startRun() {
	if (init_handler_)
		init_handler_();
}

void Runtime::
run() {
	ThreadPool::Instance().start();
	ThreadPool::Instance().joinAll();
}

}}
