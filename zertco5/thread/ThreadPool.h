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
#include <utils/updatelist/DynamicList.h>

#include "config.h"
#include "details/RWLock.hpp"
#include "Thread.h"

namespace zertcore { namespace concurrent {
using namespace zertcore::utils;
}}

namespace zertcore { namespace concurrent { namespace details {

typedef DynamicList<details::Task, 1024 * 128>
											rw_task_list_type;
typedef DynamicList<ConcurrentState::ptr, 1024 * 128>
											rw_state_list_type;

}}}

namespace zertcore { namespace concurrent {

/**
 * ThreadPool
 */
class ThreadPool
{
public:
	/**
	 * the way to locate the offset(READONLY) in map type MUST BE thread-safe!
	 * vector operator[] WAS.
 	 */
	typedef vector<details::rw_task_list_type>
											tasks_map_type;
	typedef vector<details::rw_state_list_type>
											state_map_type;

	typedef vector<Thread::ptr>				thread_ptr_list_type;

	typedef vector<init_handler_type>		init_handler_list_type;
	typedef vector<init_handler_list_type>	init_handlers_list_type;

	typedef vector<pthread_t>				thread_id_list_type;

	typedef function<size_t ()>				exclusive_handler_type;
	typedef vector<exclusive_handler_type>	exclusive_handler_list_type;

public:
	ThreadPool();
	static ThreadPool& Instance();

public:
	bool globalInit();
	bool start();

public:
	/**
	 * fetch a thread base on the index
	 * cant call when the config.concurrent.thread_num was not set up!
	 */
	bool registerInitHandler(tid_type index, const init_handler_type& start_handler);
	/**
	 * the handler MUST NOT be block
	 * if with_taskqueue was true, the handler must call runOnce() every frames
	 */
	bool registerExclusiveHandler(const exclusive_handler_type& handler,
			const init_handler_type& start_handler = init_handler_type());
	/**
	 * must called before start()
	 * unusable for now disable
	 */
//	bool addGlobalStartHandler(const init_handler_type& handler);

	void setAfterAllInitedHandler(const after_all_init_handler_type& handler);

public:
	/**
	 * when a task was pushed,
	 * the task would run in time that could't estimate.
	 * TODO: add priority support
	 */
	bool push(const details::Task& task, const priority_type& pri = PRIORITY_LOW);
	bool push(ConcurrentState::ptr state);
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
	void afterInit();
	void run();
	void stop();
	/**
	 * in the thread, call Thread::getCurrentRuntimeContext() to get the context from a Task,
	 * if the context in the Task already be set to error, the handler wont be run
	 */
	u32 runOnce();

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
	state_map_type				state_map_;

private:
	u32							thread_free_amount_,
								thread_total_amount_;
	bool						is_inited_;
	bool						is_running_;

private:
	init_handlers_list_type		init_handlers_list_;
	after_all_init_handler_type	after_all_init_handler_;
	init_handler_list_type		exclusive_init_handler_list_;
	exclusive_handler_list_type	exclusive_handler_list_;

private:
	thread_ptr_list_type		thread_list_;

private:
	spinlock_type				lock_;
	u32							inited_amount_;

private:
	static ThreadPool			instance_;
};

}}

#endif /* THREAD_H_ */
