/*
 * Ticker.cpp
 *
 *  Created on: 2015年5月6日
 *      Author: Administrator
 */

#include "UpdateServer.h"
#include "../rpc/RPCManager.h"

#include <details.h>

namespace zertcore { namespace concurrent { namespace update {
using namespace zertcore::concurrent::rpc;
}}}

namespace zertcore { namespace concurrent { namespace update {

UpdateServer::UpdateServer() : counter_(0) {
	;
}

bool UpdateServer::
globalInit(const UpdateConfig& uc) {
	interval_ = uc.interval;
	if (!interval_)
		return false;

	timer_.onExpired(bind(&UpdateServer::cbTimer, this, _1));
	return true;
}

bool UpdateServer::
start() {
	return timer_.start(interval_);
}

bool UpdateServer::
tickOnce() {
	if (state_) {
		ZCLOG(NOTE) << "State was not null, last call was not returned" << End;
		return false;
	}

	state_ = ConcurrentState::create(bind(&UpdateServer::cbTick, this, _1));
//	state_->setTimeout(interval_ - tick_type(1));

	counter_++;
	rpc::iarchiver_type params;
	params["count"] & counter_;

	if (RPCManager::Instance().asyncCall(cmd::UPDATE, params,
			rpc::rpc_callback_type(), state_, 0, RPCAllFetcher()))
		return true;

	state_ = ConcurrentState::ptr();
	return false;
}

void UpdateServer::
cbTimer(const tick_type& tick) {
	timer_.reset();
	if (!tickOnce()) {
		ZCLOG(NOTE) << "tick once failed" << End;
		return ;
	}

	ZCLOG(NOTE) << "Tick!" << End;
}

void UpdateServer::
printStatus() {
	if (state_) {
		RuntimeContext::ptr ctx = state_->getRuntimeContext();
		ZCLOG(NOTICE) << "Finished:" << state_->getCurrent() << "/" << state_->getTarget()
				<< " Error:" << (ctx? ctx->error.message: "(None)") << End;
	}
	else {
		ZCLOG(NOTICE) << "None Status" << End;
	}
}

void UpdateServer::
cbTick(RuntimeContext::ptr rc) {
	/**
	 * TODO: check the error in RC
	 */
	printStatus();
	state_ = ConcurrentState::ptr();

}

}}}
