/*
 * ThreadHandlerDetails.hpp
 *
 *  Created on: 2014��12��24��
 *      Author: Administrator
 */

#ifndef THREADHANDLERDETAILS_HPP_
#define THREADHANDLERDETAILS_HPP_

#include <pch.h>
#include <concurrent/ConcurrentState.h>

#include "../Thread.h"
#include "../ThreadHandler.h"

namespace zertcore { namespace concurrent {

template <typename HANDLER,
		typename BIND_PARAM1,
		typename BIND_PARAM2,
		typename BIND_PARAM3,
		typename BIND_PARAM4,
		typename BIND_PARAM5>
ThreadHandler<HANDLER, BIND_PARAM1,
BIND_PARAM2, BIND_PARAM3, BIND_PARAM4, BIND_PARAM5>::
ThreadHandler(const params_type& params) : params_(params) {
	thread_flags_.resize(ThreadPool::Instance().numTaskQueue());
	setThreadIndex(Thread::getCurrentTid());
}

template <typename HANDLER,
		typename BIND_PARAM1,
		typename BIND_PARAM2,
		typename BIND_PARAM3,
		typename BIND_PARAM4,
		typename BIND_PARAM5>
ThreadHandler<HANDLER, BIND_PARAM1,
BIND_PARAM2, BIND_PARAM3, BIND_PARAM4, BIND_PARAM5>::
ThreadHandler(const function_type& func, const params_type& params) : function_(func), params_(params) {
	thread_flags_.resize(ThreadPool::Instance().numTaskQueue());
	setThreadIndex(Thread::getCurrentTid());
}
/**
template <typename HANDLER,
		typename BIND_PARAM1,
		typename BIND_PARAM2,
		typename BIND_PARAM3,
		typename BIND_PARAM4,
		typename BIND_PARAM5>
ThreadHandler<HANDLER, BIND_PARAM1,
BIND_PARAM2, BIND_PARAM3, BIND_PARAM4, BIND_PARAM5>::
ThreadHandler(const self& th) : function_(th.function_), params_(th.params_), thread_flags_(th.thread_flags_) {
	;
}
*/

template <typename HANDLER,
		typename BIND_PARAM1,
		typename BIND_PARAM2,
		typename BIND_PARAM3,
		typename BIND_PARAM4,
		typename BIND_PARAM5>
void ThreadHandler<HANDLER, BIND_PARAM1,
BIND_PARAM2, BIND_PARAM3, BIND_PARAM4, BIND_PARAM5>::
setThreadIndex(const __ALL& _) {
	thread_flags_.reset();
	for (tid_type i = 0; i < thread_flags_.size(); ++i) {
		ZC_ASSERT(setThreadIndex(i));
	}
}

template <typename HANDLER,
		typename BIND_PARAM1,
		typename BIND_PARAM2,
		typename BIND_PARAM3,
		typename BIND_PARAM4,
		typename BIND_PARAM5>
void ThreadHandler<HANDLER, BIND_PARAM1,
BIND_PARAM2, BIND_PARAM3, BIND_PARAM4, BIND_PARAM5>::
setThreadIndex(const __OTHER& _) {
	ZC_ASSERT(setThreadIndex(Thread::fetchOneOtherTid()));
}

template <typename HANDLER,
		typename BIND_PARAM1,
		typename BIND_PARAM2,
		typename BIND_PARAM3,
		typename BIND_PARAM4,
		typename BIND_PARAM5>
void ThreadHandler<HANDLER, BIND_PARAM1,
BIND_PARAM2, BIND_PARAM3, BIND_PARAM4, BIND_PARAM5>::
setThreadIndex(const flag_type& flags) {
	thread_flags_ = flags;
}

template <typename HANDLER,
		typename BIND_PARAM1,
		typename BIND_PARAM2,
		typename BIND_PARAM3,
		typename BIND_PARAM4,
		typename BIND_PARAM5>
bool ThreadHandler<HANDLER, BIND_PARAM1,
BIND_PARAM2, BIND_PARAM3, BIND_PARAM4, BIND_PARAM5>::
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

template <typename HANDLER,
		typename BIND_PARAM1,
		typename BIND_PARAM2,
		typename BIND_PARAM3,
		typename BIND_PARAM4,
		typename BIND_PARAM5>
void ThreadHandler<HANDLER, BIND_PARAM1,
BIND_PARAM2, BIND_PARAM3, BIND_PARAM4, BIND_PARAM5>::
lazyThreadIndex(const u32& index) {
	ZC_ASSERT(setThreadIndex(tid_type(index % ThreadPool::Instance().size())));
}

/**
template <typename HANDLER,
		typename BIND_PARAM1,
		typename BIND_PARAM2,
		typename BIND_PARAM3,
		typename BIND_PARAM4,
		typename BIND_PARAM5>
ThreadHandler<HANDLER, BIND_PARAM1,
BIND_PARAM2, BIND_PARAM3, BIND_PARAM4, BIND_PARAM5>& ThreadHandler<HANDLER, BIND_PARAM1,
BIND_PARAM2, BIND_PARAM3, BIND_PARAM4, BIND_PARAM5>::
operator =(const ThreadHandler& handler) {
	function_ = handler.function_;
	params_ = handler.params_;
	setThreadIndex(handler.thread_flags_);
	return *this;
}
*/

template <typename HANDLER,
		typename BIND_PARAM1,
		typename BIND_PARAM2,
		typename BIND_PARAM3,
		typename BIND_PARAM4,
		typename BIND_PARAM5>
ThreadHandler<HANDLER, BIND_PARAM1,
BIND_PARAM2, BIND_PARAM3, BIND_PARAM4, BIND_PARAM5>& ThreadHandler<HANDLER, BIND_PARAM1,
BIND_PARAM2, BIND_PARAM3, BIND_PARAM4, BIND_PARAM5>::
operator =(const function_type& handler) {
	function_ = handler;
	setThreadIndex(Thread::getCurrentTid());
	return *this;
}

template <typename HANDLER,
		typename BIND_PARAM1,
		typename BIND_PARAM2,
		typename BIND_PARAM3,
		typename BIND_PARAM4,
		typename BIND_PARAM5>
bool ThreadHandler<HANDLER, BIND_PARAM1,
BIND_PARAM2, BIND_PARAM3, BIND_PARAM4, BIND_PARAM5>::
push() const {
	return ThreadPool::Instance().push(task_type(*this, getThreadIndex()));
}

}}

#endif /* THREADHANDLERDETAILS_HPP_ */
