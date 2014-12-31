/*
 * ThreadPool.cpp
 *
 *  Created on: 2014Äê11ÔÂ6ÈÕ
 *      Author: Administrator
 */

#include <pch.h>

#include <concurrent/thread/ThreadPool.h>
#include <core/Runtime.h>

namespace zertcore { namespace concurrent {

/**
 *
 */
ThreadPool::ThreadPool() : thread_amount_(0), is_running_(false) {
	;
}

bool ThreadPool::
start(u32 thread_nums) {
	thread_amount_ = thread_nums;
	is_running_ = true;
	/**
	 * offset 0 was the main thread
	 */
	for (tid_type i = 1; i <= (tid_type)thread_nums; ++i) {
		Thread::ptr thread = new Thread(i);
		assert(!pthread_create(null, null, &details::__startThreadCallback, thread));
	}

	RT.addUpdater(&ThreadPool::mainThread, this);
	return true;
}

bool ThreadPool::
addStartHandler(init_handler_type handler) {
	if (is_running_)
		return false;

	init_handler_list_.push_back(handler);
	return true;
}

bool ThreadPool::
push(const details::Task& task) {
	auto indexes = task.handler.getThreadIndex();

	for (uint i = 0; i < indexes.size() && i < thread_amount_; ++i) {
		if (indexes[i]) {
			tasks_map_[i].put(task);
		}
	}

	return true;
}

/**
bool ThreadPool::pop(const details::ThreadIndex& ti,
		details::task_list_type& tasks) {
	if (ti.index >= thread_amount_)
		return false;

	return tasks_map_[ti.index].get(tasks, true);
}


bool ThreadPool::finish(const details::task_list_type& tasks) {
	if (tasks.empty()) {
		return false;
	}

	uint index = tasks.begin()->index.index;
	if (index >= thread_amount_)
		return false;

	tasks_out_[index].put(tasks);
	return true;
}
*/

void ThreadPool::
initRun() {
	for (size_t i = 0; i < init_handler_list_.size(); ++i) {
		init_handler_list_[i]();
	}
}

u32 ThreadPool::
runOnce(bool block) {
	tid_type tid = Thread::getCurrentTid();

	details::task_list_type tasks;
	if (!tasks_map_[tid].get(tasks, block)) {
		return 0;
	}

	for (details::task_list_type::iterator it = tasks.begin();
			it != tasks.end(); ++it) {
		if (!it->handler)
			continue;

		if (it->state) {
			RunningContext& context = it->state->getRunningContext();
			if (context.error)
				continue;

			Thread::setCurrentRunningContext(context);
		}

		(it->handler)();

		if (it->state) {
			it->state->complete();
		}
	}

	return tasks.size();
}

void ThreadPool::
run(bool block) {
	while(is_running_) {
		runOnce(block);
	}
}

u32 ThreadPool::
mainThread() {
	return runOnce(false);
}

u32 ThreadPool::
size() const {
	return thread_amount_;
}

bool ThreadPool::
isValidTid(const tid_type& index) const {
	return index < thread_amount_;
}

}}

