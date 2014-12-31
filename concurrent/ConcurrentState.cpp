/*
 * Signal.cpp
 *
 *  Created on: 2014Äê10ÔÂ13ÈÕ
 *      Author: Administrator
 */
#include <pch.h>

#include <concurrent/ConcurrentState.h>
#include <concurrent/thread/ThreadPool.h>

namespace zertcore { namespace concurrent {


}}


namespace zertcore { namespace concurrent {

ConcurrentState::ConcurrentState() : target_(0) {
	current_ = 0;
}

ConcurrentState::ConcurrentState(const callback_type& handler) : target_(1) {
	cb_handler_ = handler.setParams<handler_type::handler_type>(context_);
	current_ = 0;
}

bool ConcurrentState::
onComplete(const callback_type& handler) {
	cb_handler_ = handler.setParams<handler_type::handler_type>(context_);
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
*/

ConcurrentState::
operator bool() const {
	return cb_handler_? true: false;
}

bool ConcurrentState::
complete() {
	detail::spinlock::scoped_lock guard(lock_);
	context_ = Thread::getCurrentRunningContext();

	if (++current_ >= target_) {
		if (cb_handler_) {
			cb_handler_.push();
			cb_handler_.clear();
		}
	}

	return true;
}

ConcurrentState& ConcurrentState::
operator= (const callback_type&  handler) {
	cb_handler_ = handler.setParams<handler_type::handler_type>(context_);
	target_ = 1;

	return *this;
}

bool ConcurrentState::
listen() {
	target_++;
	return true;
}

}}
