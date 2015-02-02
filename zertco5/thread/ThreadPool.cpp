/*
 * ThreadPool.cpp
 *
 *  Created on: 2014��11��6��
 *      Author: Administrator
 */

#include <pch.h>

#include "ThreadPool.h"

namespace zertcore { namespace concurrent {

/**
 * ThreadPool
 */
ThreadPool::ThreadPool() :
		tasks_map_size_(0), thread_free_amount_(0), thread_total_amount_(0),
		is_inited_(0), is_running_(false) {
	;
}

bool ThreadPool::
init(u32 thread_nums) {
	if (is_inited_)
		return false;

	is_inited_ = true;
	/**
	 * setup thread configure
	 */
	Thread::setup();

	thread_free_amount_ = thread_nums;
	thread_total_amount_ += thread_nums;
	tasks_map_size_ += thread_nums;

	/**
	 * some threads were in exclusive mode that didn't need tasks
	 */
	ZC_ASSERT(thread_total_amount_ >= tasks_map_size_);

	tasks_map_.resize(tasks_map_size_);
	for (tid_type i = 0; i < (tid_type)tasks_map_size_; ++i) {
		tasks_map_[i] = details::rw_task_list_type::create();
	}

	return true;
}

bool ThreadPool::
start() {
	if (is_running_)
		return false;

	is_running_ = true;
	/**
	 * thread 0 was the main thread
	 */
	for (tid_type i = 0; i < (tid_type)thread_total_amount_; ++i) {
		Thread::ptr thread = Thread::create(i);
		pthread_t pid;
		ZC_ASSERT(!pthread_create(&pid, null, &details::__startThreadCallback, thread));

		thread_ids_.push_back(pid);
	}

	return true;
}

bool ThreadPool::
registerExclusiveHandler(const exclusive_handler_type& handler, bool with_taskqueue) {
	if (is_running_)
		return false;

	if (with_taskqueue) {
		tasks_map_size_++;
		exclusive_handler_list_.insert(exclusive_handler_list_.begin(), handler);
	}
	else {
		exclusive_handler_list_.push_back(handler);
	}

	thread_total_amount_++;
	return true;
}

bool ThreadPool::
addStartHandler(const init_handler_type& handler) {
	if (is_running_)
		return false;

	init_handler_list_.push_back(handler);
	return true;
}

bool ThreadPool::
push(const details::Task& task) {
	auto indexes = task.flags;
	bool flag = false;

	for (uint i = 0; i < indexes.size() && i < tasks_map_size_; ++i) {
		if (indexes[i]) {
			flag = true;

			::printf("Put %u in tasks\n", i);

			tasks_map_[i]->put(task);
		}
	}

	return flag;
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
		spinlock_guard_type guard(lock_);
		init_handler_list_[i]();
	}

	tid_type tid = Thread::getCurrentTid();
	if (tid >= thread_free_amount_) {
		tid -= (tid_type)thread_free_amount_;

		ZC_ASSERT(tid < (tid_type)exclusive_handler_list_.size());
		exclusive_handler_list_[tid]();
	}
}

u32 ThreadPool::
runOnce(bool block) {
	if (!is_running_)
		return 0;

	tid_type tid = Thread::getCurrentTid();
	ZC_ASSERT(tid < tasks_map_.size());

	details::task_list_type tasks;
	if (!tasks_map_[tid]->get(tasks, block)) {
		return 0;
	}

	::printf("Get From %u\n", tid);

	for (details::task_list_type::iterator it = tasks.begin();
			it != tasks.end(); ++it) {
		if (!it->handler)
			continue;

		if (it->state) {
			if (!it->state->setupRunningContext()) {
				::printf("Context Error\n");
				/**
				 * if the status has set to error, flag the complete and abandon to run
				 */
				it->state->complete();
				continue;
			}
		}

		::printf("Run handler\n");
		(it->handler)();

		if (it->state) {
			::printf("it->state->complete()\n");
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

void ThreadPool::
joinAll() {
	std::for_each(thread_ids_.begin(), thread_ids_.end(), [] (const pthread_t& id) {
		pthread_join(id, null);
	});
}

bool ThreadPool::
isUsableTid(const tid_type& index) const {
	return index < tasks_map_size_;
}

}}

