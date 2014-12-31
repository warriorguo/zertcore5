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

#include <concurrent/config.h>
#include <concurrent/details/RWLock.hpp>
#include <concurrent/ConcurrentState.h>

#include <concurrent/thread/Thread.h>

namespace zertcore { namespace concurrent {
using namespace zertcore::utils;
}}

namespace zertcore { namespace concurrent {

/**
 * ThreadPool
 */
class ThreadPool :
		public Singleton<ThreadPool>
{
public:
	typedef vector<details::rw_task_list_type>
											tasks_map_type;
	typedef vector<Thread::ptr>				thread_ptr_list_type;

	typedef function<void ()>				init_handler_type;
	typedef vector<init_handler_type>		init_handler_list_type;

public:
	ThreadPool();

public:
	bool start(u32 thread_nums);

public:
	/**
	 * must called before start()
	 */
	bool addStartHandler(init_handler_type handler);

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
	 * Called by thread
	 */
	void initRun();
	void run(bool is_block);
	/**
	 * in the thread, call Thread::getCurrentRunningContext() to get the context from a Task,
	 * if the context in the Task already be set to error, the handler wont be run
	 */
	u32 runOnce(bool is_block);

public:
	/**
	 * Main thread
	 */
	u32 mainThread();

public:
	u32 size() const;
	bool isValidTid(const tid_type& index) const;

private:
	tasks_map_type				tasks_map_;

private:
	uint						thread_amount_;
	bool						is_running_;

private:
	init_handler_list_type		init_handler_list_;

private:
	thread_ptr_list_type		thread_list_;
};

}}

#include <concurrent/thread/details/ThreadHandlerDetails.hpp>

#endif /* THREAD_H_ */
