/*
 * ThreadPool.cpp
 *
 *  Created on: 2014��11��6��
 *      Author: Administrator
 */

#include <pch.h>
#include <config/Config.h>
#include <utils/time/TimerManager.h>
#include <log/Log.h>
#include <concurrent/coroutine/Coroutine.h>
#include <details.h>

#include "ThreadPool.h"
#include "ThreadHandler.h"
#include "details/ThreadHandlerDetails.hpp"

namespace zertcore { namespace concurrent {
using namespace zertcore::time_utils;

/**
 * __contextCall
 */
#ifndef ZC_DISABLE_COROUTINE

void __contextCall(cofcell_type::ptr fc, void* params) {
	details::Task* task = (details::Task *)params;
	ConcurrentState* state = task->state.get();

	Coroutine::Instance().setLastFCell(fc);

	task->handler();

	if (state) {
		state->complete();
	}

	fc->finished = true;
//	Coroutine::Instance().jumpBack(fc);
}

#endif

}}

namespace zertcore { namespace concurrent {
ThreadPool						ThreadPool::instance_;

/**
 * ThreadPool
 */
ThreadPool::ThreadPool() :
		tasks_map_size_(0), thread_free_amount_(0), thread_total_amount_(0),
		is_inited_(0), is_running_(false), inited_amount_(0) {
	;
}

ThreadPool& ThreadPool::Instance() {
	return instance_;
}

bool ThreadPool::
globalInit() {
	if (is_inited_)
		return false;

	is_inited_ = true;
	/**
	 * setup thread configure
	 */
	Thread::setup();

	thread_free_amount_ = config.concurrent.thread_nums;
	thread_total_amount_ += config.concurrent.thread_nums;
	tasks_map_size_ = thread_total_amount_;

	tasks_map_.resize(tasks_map_size_);
	state_map_.resize(tasks_map_size_);

	/**
	for (tid_type i = 0; i < (tid_type)tasks_map_.size(); ++i) {
		tasks_map_[i] = details::rw_task_list_type::create();
		state_map_[i] = details::rw_state_list_type::create();
	}
	*/

	return true;
}

bool ThreadPool::
start() {
	if (is_running_)
		return false;

	is_running_ = true;

	for (tid_type i = 1; i <= (tid_type)thread_total_amount_; ++i) {
		tasks_map_[i - 1].prepareHead();
		tasks_map_[i - 1].prepareTail();

		state_map_[i - 1].prepareHead();
		state_map_[i - 1].prepareTail();

		Thread::ptr thread = Thread::create(i);
		thread_list_.push_back(thread);
	}

	return true;
}

bool ThreadPool::
registerInitHandler(tid_type tid, const init_handler_type& start_handler) {
	if (init_handlers_list_.empty()) {
		if (!config.concurrent.thread_nums) {
			ZCLOG(FATAL) << "config.concurrent.thread_nums was not setup before call registerInitHandler()" << End;
		}

		init_handlers_list_.resize(config.concurrent.thread_nums);
	}

	if (!tid)
		return false;

	if (is_running_) {
		return goThread(tid, start_handler);
	}

	tid--;

	if (tid >= config.concurrent.thread_nums) {
		ZCLOG(ERROR) << "index = " << tid << " was larger than config.concurrent.thread_nums="
				<< config.concurrent.thread_nums << End;
		return false;
	}

	init_handlers_list_[tid].push_back(start_handler);
	return true;
}

bool ThreadPool::
registerExclusiveHandler(const exclusive_handler_type& handler, const init_handler_type& start_handler) {
	if (is_running_)
		return false;

	exclusive_handler_list_.push_back(handler);
	exclusive_init_handler_list_.push_back(start_handler);

	thread_total_amount_++;
	return true;
}

void ThreadPool::
setAfterAllInitedHandler(const after_all_init_handler_type& handler) {
	after_all_init_handler_ = handler;
}

bool ThreadPool::
push(const details::Task& task, const priority_type& pri) {
	bool flag = false;
#ifdef ZC_ENABLE_THREADHANDLER_MULTICAST
	auto indexes = task.flags;

	for (uint i = 0; i < indexes.size() && i < tasks_map_size_; ++i)
	{
		if (indexes[i]) {
#else
	{
		tid_type i = task.flags - 1;
		if (i < tasks_map_size_) {
#endif
			flag = true;

			if (pri == PRIORITY_HIGH)
				tasks_map_[i].addHead(task);
			else
				tasks_map_[i].addTail(task);
		}
	}

	if (!flag) {
		ZCLOG(WARNING) << "Failed to push a handler into ThreadPool:" << task.flags << End;
	}

	return flag;
}

bool ThreadPool::
push(ConcurrentState::ptr state) {
	tid_type tid = state->getTid() - 1;

	ZC_DEBUG_ASSERT(tid < tasks_map_size_);

	state_map_[tid].addTail(state);
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
	tid_type tid = Thread::getCurrentTid() - 1;

	ZC_ASSERT(tid < tasks_map_.size());

	if (tid >= thread_free_amount_) {
		tid_type thread_id = tid - (tid_type)thread_free_amount_;

		ZC_ASSERT(thread_id < exclusive_init_handler_list_.size());

		if (exclusive_init_handler_list_[thread_id])
			exclusive_init_handler_list_[thread_id]();
	}
	else {
		if (!init_handlers_list_.empty()) {
			ZC_ASSERT(tid < init_handlers_list_.size());

			for (tid_type i = 0; i < init_handlers_list_[tid].size(); ++i) {
				init_handlers_list_[tid][i]();
			}
		}
	}

	afterInit();
}

void ThreadPool::
afterInit() {
	spinlock_guard_type guard(lock_);
	inited_amount_++;

	if (inited_amount_ == thread_total_amount_) {
		if (after_all_init_handler_)
			after_all_init_handler_();
	}
}

u32 ThreadPool::
runOnce() {
	u32 exec_amount = 0;

	if (!is_running_)
		return 1;

	tid_type tid = Thread::getCurrentTid() - 1;

	ZC_ASSERT( tid < tasks_map_.size() );

	if (tid >= thread_free_amount_) {
		tid_type thread_id = tid - (tid_type)thread_free_amount_;

		ZC_ASSERT(thread_id < (tid_type)exclusive_handler_list_.size());
		exclusive_handler_list_[thread_id]();
	}

	size_t list_size = 0;
	details::rw_task_list_type::container_ptr tasks_ptr = tasks_map_[tid].getHead(list_size);
	if (tasks_ptr) {
		for (auto it = tasks_ptr->begin(); list_size && it != tasks_ptr->end(); --list_size, ++it) {
			if (!it->handler)
				continue;

			if (it->state) {
				if (it->state->isError()) {
					// ZCLOG(WARNING) << "Context was error" << End;
					/**
					 * if the status has set to error, flag the complete and abandon to run
					 */
					it->state->complete();
					continue;
				}
			}

			/**
			 * setup concurrent state for the running handler
			 */
			Thread::setCurrentConcurrentState(it->state);
			Thread::setupCurrentRuntimeContext();

#ifndef ZC_DISABLE_COROUTINE

			cofcell_type::ptr fc = cofcell_type::create();
			Coroutine::Instance().make(fc, __contextCall, fc, (void *)& (*it));
			Coroutine::Instance().jumpTo(fc);

			if (fc->finished) {
				fc->release();
			}

#else

			it->handler();

			if (it->state) {
				it->state->complete();
			}

#endif

			exec_amount++;
		}
		tasks_map_[tid].releaseHead(tasks_ptr);
	}

	tasks_ptr = tasks_map_[tid].getTail(list_size);
	if (tasks_ptr) {
		for (auto it = tasks_ptr->begin(); list_size && it != tasks_ptr->end(); --list_size, ++it) {
			if (!it->handler)
				continue;

			if (it->state) {
				if (it->state->isError()) {
					// ZCLOG(WARNING) << "Context was error" << End;
					/**
					 * if the status has set to error, flag the complete and abandon to run
					 */
					it->state->complete();
					continue;
				}
			}

			/**
			 * setup concurrent state for the running handler
			 */
			Thread::setCurrentConcurrentState(it->state);
			Thread::setupCurrentRuntimeContext();

#ifndef ZC_DISABLE_COROUTINE

			cofcell_type::ptr fc = cofcell_type::create();
			Coroutine::Instance().make(fc, __contextCall, fc, (void *)& (*it));
			Coroutine::Instance().jumpTo(fc);

			if (fc->finished) {
				fc->release();
			}

#else

			it->handler();

			if (it->state) {
				it->state->complete();
			}

#endif

			exec_amount++;
		}
		tasks_map_[tid].releaseTail(tasks_ptr);
	}
	/**
	 * update the timer every frame
	 */
	exec_amount = TimerManager::Instance().update();

#ifndef ZC_DISABLE_COROUTINE

	details::rw_state_list_type::container_ptr states_ptr = state_map_[tid].getHead(list_size);
	if (states_ptr) {
		for (auto it = states_ptr->begin(); list_size && it != states_ptr->end(); --list_size, ++it) {
			ConcurrentState::ptr state = *it;
			state->resume();
			state->releaseCoroutineCtx();
			exec_amount++;
		}
		state_map_[tid].releaseHead(states_ptr);
	}
	states_ptr = state_map_[tid].getTail(list_size);
	if (states_ptr) {
		for (auto it = states_ptr->begin(); list_size && it != states_ptr->end(); --list_size, ++it) {
			ConcurrentState::ptr state = *it;
			state->resume();
			state->releaseCoroutineCtx();
			exec_amount++;
		}
		state_map_[tid].releaseTail(states_ptr);
	}

#endif

	return exec_amount;
}

void ThreadPool::
run() {
	for (ulong i = 0; is_running_;) {
		if (!runOnce()) ++i; else i = 0;
		if (i < 30) continue;
		else if (i < 100) usleep(0);
		else usleep(1);
	}
}

void ThreadPool::
stop() {
	is_running_ = false;
}

void ThreadPool::
joinAll() {
	for (auto it = thread_list_.begin(); it != thread_list_.end(); ++it) {
		(*it)->join();
	}
}

bool ThreadPool::
isUsableTid(const tid_type& index) const {
	return index && index <= tasks_map_size_;
}

}}

