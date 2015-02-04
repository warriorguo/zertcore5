/*
 * Signal.cpp
 *
 *  Created on: 2014��10��13��
 *      Author: Administrator
 */
#include <pch.h>

#include <thread/ThreadPool.h>

#include "ConcurrentState.h"

namespace zertcore { namespace concurrent {


}}


namespace zertcore { namespace concurrent {

ConcurrentState::ConcurrentState() : target_(0), cb_handler_(cbt_handler_type::params_type({context_})) {
	current_ = 0;
}

ConcurrentState::ConcurrentState(const callback_type& handler) :
		target_(0), cb_handler_(handler, cbt_handler_type::params_type({context_})) {
	current_ = 0;
}

bool ConcurrentState::
onComplete(const callback_type& handler) {
	cb_handler_ = handler;
	return true;
}

/**
handler_type& ConcurrentState::getCompleteHandler() {
	ConcurrentStateInfoProvider::Cell::ptr cell = ConcurrentStateInfoProvider::Instance().get(index_);
	if (!cell) {
		throw Error("ConcurrentStateInfoProvider::Instance().get is NULL");
	}

	return cb_handler_;
}


void ConcurrentState::
handleCallback() {
	ZC_ASSERT(cb_handler_);
	cb_handler_(context_);
}
*/

ConcurrentState::
operator bool() const {
	return cb_handler_? true: false;
}

bool ConcurrentState::
complete() {
	spinlock_guard_type guard(lock_);
	context_ = Thread::getCurrentRunningContext();

	if (++current_ >= target_) {
		if (cb_handler_) {
			/**
			handler_type cb(bind(&ConcurrentState::handleCallback, thisPtr()));
			cb.setThreadIndex(cb_handler_.getThreadIndex());

			ZC_ASSERT(cb.push());
			*/
			ZC_ASSERT(cb_handler_.push());
		}
	}

	return true;
}

ConcurrentState& ConcurrentState::
operator= (const callback_type& handler) {
	cb_handler_ = handler;
	target_ = 1;

	return *this;
}

bool ConcurrentState::
setupRunningContext() {
	if (context_.error)
		return false;

	Thread::setCurrentRunningContext(context_);
	return true;
}

bool ConcurrentState::
listen() {
	target_++;
	return true;
}

}}
