/*
 * ThreadHandlerDetails.hpp
 *
 *  Created on: 2014Äê12ÔÂ24ÈÕ
 *      Author: Administrator
 */

#ifndef THREADHANDLERDETAILS_HPP_
#define THREADHANDLERDETAILS_HPP_

#include <pch.h>

#include <concurrent/thread/Thread.h>
#include <concurrent/thread/ThreadPool.h>
#include <concurrent/thread/ThreadHandler.h>

namespace zertcore { namespace concurrent {

template <typename HANDLER>
ThreadHandler<HANDLER>::ThreadHandler() {
	setThreadIndex(Thread::getCurrentTid());;
}

template <typename HANDLER>
ThreadHandler<HANDLER>::ThreadHandler(const function_type& func) : function_(func) {
	setThreadIndex(Thread::getCurrentTid());
}

template <typename HANDLER>
void ThreadHandler<HANDLER>::
setThreadIndex(const __ALL& _) {
	for (tid_type i = 0; i < thread_flags_.size(); ++i) {
		setThreadIndex(i);
	}
}

template <typename HANDLER>
void ThreadHandler<HANDLER>::
setThreadIndex(const __OTHER& _) {
	setThreadIndex(Thread::fetchOneOtherTid());
}

template <typename HANDLER>
void ThreadHandler<HANDLER>::
setThreadIndex(const flag_type& flags) {
	thread_flags_ = flags;
}

template <typename HANDLER>
bool ThreadHandler<HANDLER>::
setThreadIndex(const tid_type& index) {
	if (!ThreadPool::Instance().isValidTid(index)) {
		return false;
	}

	thread_flags_[index] = true;
	return true;
}

template <typename HANDLER>
ThreadHandler<HANDLER>& ThreadHandler<HANDLER>::
operator =(const ThreadHandler& handler) {
	function_ = handler.function_;
	return *this;
}

template <typename HANDLER>
ThreadHandler<HANDLER>& ThreadHandler<HANDLER>::
operator =(const function_type& handler) {
	function_ = handler;
	return *this;
}

template <typename HANDLER>
void ThreadHandler<HANDLER>::push() const {
	ThreadPool::Instance().push(*this);
}

}}

#endif /* THREADHANDLERDETAILS_HPP_ */
