/*
 * Signal.h
 *
 *  Created on: 2014Äê10ÔÂ13ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_SIGNAL_H_
#define ZERTCORE_SIGNAL_H_

#include <pch.h>

#include <utils/types.h>
#include <utils/Singleton.h>

#include <object/PoolObject.h>

#include <concurrent/config.h>
#include <concurrent/thread/ThreadHandler.h>
#include <concurrent/details/RWLock.hpp>

namespace zertcore { namespace concurrent {
using namespace zertcore::utils;
using namespace zertcore::object;
}}

namespace zertcore { namespace concurrent {

typedef ThreadHandler<void (const RunningContext&)>
											callback_type;
typedef ThreadHandler<void ()>				handler_type;

/**
 * ConcurrentContext.
 * for several tasks
 */
class ConcurrentState :
		public PoolObject<ConcurrentState>
{
public:
	typedef ObjectTraits<ConcurrentState>::ptr
											ptr;

public:
	ConcurrentState();
	ConcurrentState(const callback_type& handler);

public:
	/**
	 * set callback when all complete
	 */
	bool onComplete(const callback_type& handler);

public:
	operator bool() const;

public:
	bool complete();
	bool listen();

public:
	ConcurrentState& operator= (const callback_type& handler);

public:
	RunningContext& getRunningContext() {
		return context_;
	}

private:
	atomic<u32>					current_;
	u32							target_;

	handler_type				cb_handler_;
	RunningContext				context_;

	ZC_TO_STRING(
		"current:" << (uint)current_ <<
		"target:" << target_
	);

private:
	detail::spinlock			lock_;
};

namespace details {
/**
 * Task
 */
struct Task
{
	handler_type				handler;
	ConcurrentState::ptr		state;

	Task(const handler_type& h) : handler(h) {}
};

typedef list<Task>							task_list_type;
typedef RWLock<task_list_type>				rw_task_list_type;
}

}}

namespace zertcore { namespace concurrent{
typedef details::Task						task_type;
}}


#endif /* SIGNAL_H_ */
