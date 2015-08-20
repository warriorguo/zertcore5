/*
 * ThreadHandlerDetails.hpp
 *
 *  Created on: 2014��12��24��
 *      Author: Administrator
 */

#ifndef ZERTCORE_THREADHANDLERDETAILS_HPP_
#define ZERTCORE_THREADHANDLERDETAILS_HPP_

#include <pch.h>

#include <log/Log.h>
#include <concurrent/ConcurrentState.h>

#include "../Thread.h"
#include "../ThreadHandler.h"
#include "../ThreadPool.h"

namespace zertcore { namespace concurrent {

template <typename HANDLER>
ThreadHandler<HANDLER>::
ThreadHandler() : priority_(PRIORITY_LOW) {
#ifdef ZC_ENABLE_THREADHANDLER_MULTICAST
	thread_flags_.resize(ThreadPool::Instance().numTaskQueue());
#endif
	if (Thread::isSetup())
		setThreadIndex(Thread::getCurrentTid());
}

template <typename HANDLER>
template <typename CALLABLE>
ThreadHandler<HANDLER>::
ThreadHandler(const CALLABLE& func) : function_(func), priority_(PRIORITY_LOW) {
#ifdef ZC_ENABLE_THREADHANDLER_MULTICAST
	thread_flags_.resize(ThreadPool::Instance().numTaskQueue());
#endif
	ZC_DEBUG_ASSERT(Thread::isSetup());
	setThreadIndex(Thread::getCurrentTid());
}

#ifdef ZC_ENABLE_THREADHANDLER_MULTICAST
template <typename HANDLER>
template <typename CALLABLE>
ThreadHandler<HANDLER>::
ThreadHandler(const CALLABLE& func, const tid_type& tid) :
	function_(func), priority_(PRIORITY_LOW) {
	thread_flags_.resize(ThreadPool::Instance().numTaskQueue());
	setThreadIndex(tid);
}
#endif

template <typename HANDLER>
template <typename CALLABLE>
ThreadHandler<HANDLER>::
ThreadHandler(const CALLABLE& func, const flag_type& tids) :
	function_(func), priority_(PRIORITY_LOW) {
#ifdef ZC_ENABLE_THREADHANDLER_MULTICAST
	thread_flags_.resize(ThreadPool::Instance().numTaskQueue());
#endif
	thread_flags_ = tids;
}

#ifdef ZC_ENABLE_THREADHANDLER_MULTICAST
template <typename HANDLER>
void ThreadHandler<HANDLER>::
setThreadIndex(const __ALL& _) {
	thread_flags_.reset();
	for (tid_type i = 0; i < thread_flags_.size(); ++i) {
		ZC_ASSERT(setThreadIndex(i));
	}
}
#endif

template <typename HANDLER>
void ThreadHandler<HANDLER>::
setThreadIndex(const __OTHER& _) {
	ZC_ASSERT(setThreadIndex(Thread::fetchOneOtherTid()));
}

#ifdef ZC_ENABLE_THREADHANDLER_MULTICAST
template <typename HANDLER>
void ThreadHandler<HANDLER>::
setThreadIndex(const flag_type& flags) {
	thread_flags_ = flags;
}
#endif

template <typename HANDLER>
bool ThreadHandler<HANDLER>::
setThreadIndex(const tid_type& index) {
	if (!ThreadPool::Instance().isUsableTid(index)) {
		ZCLOG(ERROR) << "Try to put " << index << " in Error Thread[task queue size="
				<< ThreadPool::Instance().numTaskQueue() << "]" << End;
		return false;
	}

#ifdef ZC_ENABLE_THREADHANDLER_MULTICAST
	thread_flags_.reset();
	ZC_ASSERT(index < thread_flags_.size());
	thread_flags_[index] = true;
#else
	thread_flags_ = index;
#endif
	return true;
}

template <typename HANDLER>
void ThreadHandler<HANDLER>::
lazyThreadIndex(const u32& index) {
	ZC_ASSERT(setThreadIndex(Thread::lazyTid(index)));
}

#ifdef ZC_ENABLE_THREADHANDLER_STATE

template <typename HANDLER>
void ThreadHandler<HANDLER>::
setupState(current_state_ptr state) {
	state->listen();
	current_state_list_.push_back(state);
}

template <typename HANDLER>
void ThreadHandler<HANDLER>::
completeState() const {
	for (auto it = current_state_list_.begin();
			it != current_state_list_.end(); ++it) {
		ConcurrentState::ptr state = *it;
		ZC_ASSERT(state);

		state->complete();
	}
}

#endif

template <typename HANDLER>
ThreadHandler<HANDLER>& ThreadHandler<HANDLER>::
operator =(const ThreadHandler& handler) {
	function_ = handler.function_;
	params_ = handler.params_;
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
bool ThreadHandler<HANDLER>::
push() const {
	return ThreadPool::Instance().push(task_type(*this, getThreadIndex()), priority_);
}

template <typename HANDLER>
bool ThreadHandler<HANDLER>::
push(const params_type& params) const {
	setParams(params);
	return ThreadPool::Instance().push(task_type(*this, getThreadIndex()), priority_);
}

template <typename HANDLER>
void ThreadHandler<HANDLER>::
operator() () const {
	if (!callable()) {
		ZCLOG(ERROR) << "Function was Uncallabled" << End;
		return ;
	}

#ifndef ZC_RELEASE
#ifdef ZC_ENABLE_THREADHANDLER_MULTICAST
	tid_type tid = Thread::getCurrentTid();
	ZC_ASSERT(thread_flags_.size() > tid);
	ZC_ASSERT(thread_flags_[tid]);
#endif

#endif

	callWithParams(function_, *(params_.get()));

#ifdef ZC_ENABLE_THREADHANDLER_STATE
	completeState();
#endif
}

}}

#endif /* THREADHANDLERDETAILS_HPP_ */
