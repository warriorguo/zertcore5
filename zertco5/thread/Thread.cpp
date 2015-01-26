/*
 * Thread.cpp
 *
 *  Created on: 2014Äê12ÔÂ23ÈÕ
 *      Author: Administrator
 */

#include <pch.h>

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
ThreadLocal<RunningContext>		Thread::thread_context_;
}}

namespace zertcore {namespace concurrent {

Thread::Thread(const tid_type& tid) : tid_(tid) {}
Thread::~Thread() {}

void Thread::
setCurrentRunningContext(const RunningContext& context) {
	thread_context_.load() = context;
}

RunningContext& Thread::
getCurrentRunningContext() {
	return thread_context_.load();
}

tid_type Thread::
getCurrentTid() {
	return thread_index_.load();
}

tid_type Thread::
fetchOneOtherTid() {
	return (getCurrentTid() + 1) % ThreadPool::Instance().size();
}

void Thread::
init() {
	thread_index_.load() = tid_;

	ThreadPool::Instance().initRun();
}

void Thread::
startRun() {
	ThreadPool::Instance().run(true);
}

} /* namespace concurrent */} /* namespace zertcore */
