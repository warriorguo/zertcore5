/*
 * ThreadHandlerDetails.hpp
 *
 *  Created on: 2014Äê12ÔÂ24ÈÕ
 *      Author: Administrator
 */

#ifndef THREADHANDLERDETAILS_HPP_
#define THREADHANDLERDETAILS_HPP_

#include <pch.h>

#include "../Thread.h"
#include "../ThreadPool.h"
#include "../ThreadHandler.h"

namespace zertcore { namespace concurrent {

template <typename HANDLER>
ThreadHandler<HANDLER>::ThreadHandler() {
	thread_flags_.resize(ThreadPool::Instance().numTaskQueue());
	setThreadIndex(Thread::getCurrentTid());
}

template <typename HANDLER>
ThreadHandler<HANDLER>::ThreadHandler(const function_type& func) : function_(func) {
	thread_flags_.resize(ThreadPool::Instance().numTaskQueue());
	setThreadIndex(Thread::getCurrentTid());
}

template <typename HANDLER>
void ThreadHandler<HANDLER>::
setThreadIndex(const __ALL& _) {
	thread_flags_.reset();
	for (tid_type i = 0; i < thread_flags_.size(); ++i) {
		ZC_ASSERT(setThreadIndex(i));
	}
}

template <typename HANDLER>
void ThreadHandler<HANDLER>::
setThreadIndex(const __OTHER& _) {
	ZC_ASSERT(setThreadIndex(Thread::fetchOneOtherTid()));
}

template <typename HANDLER>
void ThreadHandler<HANDLER>::
setThreadIndex(const flag_type& flags) {
	thread_flags_ = flags;
}

template <typename HANDLER>
bool ThreadHandler<HANDLER>::
setThreadIndex(const tid_type& index) {
	if (!ThreadPool::Instance().isUsableTid(index)) {
		::printf("try to put %u %u\n", index, ThreadPool::Instance().numTaskQueue());
		return false;
	}

	thread_flags_.reset();

	ZC_ASSERT(index < thread_flags_.size());
	thread_flags_[index] = true;
	return true;
}

template <typename HANDLER>
void ThreadHandler<HANDLER>::
lazyThreadIndex(const u32& index) {
	ZC_ASSERT(setThreadIndex(tid_type(index % ThreadPool::Instance().size())));
}

template <typename HANDLER>
ThreadHandler<HANDLER>& ThreadHandler<HANDLER>::
operator =(const ThreadHandler& handler) {
	function_ = handler.function_;
	setThreadIndex(handler.thread_flags_);
	return *this;
}

template <typename HANDLER>
ThreadHandler<HANDLER>& ThreadHandler<HANDLER>::
operator =(const function_type& handler) {
	function_ = handler;
	setThreadIndex(Thread::getCurrentTid());
	return *this;
}

template <typename HANDLER>
bool ThreadHandler<HANDLER>::push() const {
	return ThreadPool::Instance().push(*this);
}

}}

#endif /* THREADHANDLERDETAILS_HPP_ */
