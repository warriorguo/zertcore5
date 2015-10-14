/*
 * Thread.cpp
 *
 *  Created on: 2014��12��23��
 *      Author: Administrator
 */

#include <pch.h>
#include <config/Config.h>

#include "Thread.h"
#include "ThreadPool.h"

namespace zertcore {namespace concurrent {namespace details{

void* __startThreadCallback(void * p) {
	Thread::ptr thread = (Thread::ptr)p;
	if (thread) {
		/**
		 * Init
		 */
		thread->init();

		/**
		 * Run
		 */
		thread->startRun();
	}

	return null;
}

}}}

namespace zertcore {namespace concurrent {
ThreadLocal<tid_type>			Thread::thread_index_;
ThreadLocal<RuntimeContext>		Thread::thread_context_;
ThreadLocal<ConcurrentState::ptr>
								Thread::thread_state_;
bool							Thread::is_setup_ = false;
}}

namespace zertcore {

RuntimeContext& threadContext() {
	return concurrent::Thread::getCurrentRuntimeContext();
}

concurrent::ConcurrentState::ptr concurrentState() {
	return concurrent::Thread::getCurrentConcurrentState();
}

}

namespace zertcore {namespace concurrent {

Thread::Thread(const tid_type& tid) : tid_(tid) {
	ZC_ASSERT(!pthread_create(&pid_, nullptr, &details::__startThreadCallback, this));
}
Thread::~Thread() {}

void Thread::
setupCurrentRuntimeContext() {
	thread_context_.load().clear();
}

RuntimeContext& Thread::
getCurrentRuntimeContext() {
	return thread_context_.load();
}

tid_type Thread::
getCurrentTid() {
	ZC_ASSERT(is_setup_);
	return thread_index_.load();
}

tid_type Thread::
fetchOneOtherTid() {
	return ((getCurrentTid() + 1) % ThreadPool::Instance().size()) + 1;
}

ConcurrentState::ptr Thread::
getCurrentConcurrentState() {
	return thread_state_.load();
}
void Thread::
setCurrentConcurrentState(ConcurrentState::ptr state) {
	thread_state_.load() = state;
}

tid_type Thread::
lazyTid(const tid_type& tid) {
	return (tid % config.concurrent.thread_nums) + 1;
}

void Thread::
setup() {
	is_setup_ = true;
}

void Thread::
join() {
	pthread_join(pid_, nullptr);
}

void Thread::
init() {
	thread_index_.load() = tid_;

	ThreadPool::Instance().initRun();
}

void Thread::
startRun() {
	ThreadPool::Instance().run();
}

} /* namespace concurrent */} /* namespace zertcore */
