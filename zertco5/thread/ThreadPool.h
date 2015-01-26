/*
 * Thread.h
 *
 *  Created on: 2014-10-6
 *      Author: Administrator
 */

#ifndef ZERTCORE_THREADPOOL_H_
#define ZERTCORE_THREADPOOL_H_

#include <pch.h>

#include <utils/types.h>
#include <utils/Singleton.h>

#include <concurrent/ConcurrentState.h>

#include "details/RWLock.hpp"
#include "Thread.h"

namespace zertcore { namespace concurrent {
using namespace zertcore::utils;
}}

namespace zertcore { namespace concurrent { namespace details {
typedef RWLock<task_list_type>				rw_task_list_type;
}}}

namespace zertcore { namespace concurrent {

/**
 * ThreadPool
 */
class ThreadPool :
		public Singleton<ThreadPool>
{
public:
	typedef vector<details::rw_task_list_type::ptr>
											tasks_map_type;
	typedef vector<Thread::ptr>				thread_ptr_list_type;

	typedef function<void ()>				init_handler_type;
	typedef vector<init_handler_type>		init_handler_list_type;
	typedef vector<pthread_t>				thread_id_list_type;

	typedef function<void ()>				exclusive_handler_type;
	typedef vector<exclusive_handler_type>	exclusive_handler_list_type;

public:
	ThreadPool();

public:
	bool init(u32 thread_nums);
	bool start();

public:
	/**
	 * the handler must be block by itself
	 * if with_taskqueue was true, the handler must call runOnce() every frames
	 */
	bool registerExclusiveHandler(const exclusive_handler_type& handler,
			bool with_taskqueue = false);
	/**
	 * must called before start()
	 */
	bool addStartHandler(const init_handler_type& handler);

public:
	/**
	 * when a task was pushed,
	 * the task would run in time that could't estimate.
	 */
	bool push(const details::Task& task);
//	bool push(const details::task_list_type& tasks);

/**
private:
	bool pop(const details::ThreadIndex& ti, details::task_list_type& tasks);
	bool finish(const details::task_list_type& tasks);
*/

public:
	/**
	 * Called by thread, main thread would block here
	 */
	void initRun();
	void run(bool is_block);
	/**
	 * in the thread, call Thread::getCurrentRunningContext() to get the context from a Task,
	 * if the context in the Task already be set to error, the handler wont be run
	 */
	u32 runOnce(bool is_block = false);

public:
	void joinAll();

public:
	/**
	 * Free thread amount
	 */
	u32 size() const {
		return thread_free_amount_;
	}
	u32 totalSize() const {
		return thread_total_amount_;
	}

	u32 numTaskQueue() const {
		return tasks_map_size_;
	}

	bool isUsableTid(const tid_type& index) const;

private:
	tasks_map_type				tasks_map_;
	u32							tasks_map_size_;

private:
	u32							thread_free_amount_,
								thread_total_amount_;
	bool						is_inited_;
	bool						is_running_;

private:
	init_handler_list_type		init_handler_list_;
	exclusive_handler_list_type	exclusive_handler_list_;

private:
	thread_ptr_list_type		thread_list_;
	thread_id_list_type			thread_ids_;

private:
	spinlock_type				lock_;
};

}}

#include "details/ThreadHandlerDetails.hpp"

#endif /* THREAD_H_ */
