/*
 * Signal.h
 *
 *  Created on: 2014Äê10ÔÂ13ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_CONCURRENTSTATE_H_
#define ZERTCORE_CONCURRENTSTATE_H_

#include <pch.h>

#include <utils/types.h>
#include <utils/Singleton.h>

#include <object/PoolObject.h>
#include <thread/ThreadHandler.h>

#include "config.h"

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

	virtual ~ConcurrentState() {
		::printf("~ConcurrentState()\n");
	}

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
	bool setupRunningContext();

private:
	void handleCallback();

private:
	u32							current_;
	u32							target_;

	callback_type				cb_handler_;
	RunningContext				context_;

	ZC_TO_STRING(
		"current:" << (uint)current_ <<
		"target:" << target_
	);

private:
	spinlock_type				lock_;
};

namespace details {
/**
 * Task
 */
struct Task
{
	handler_type				handler;
	ConcurrentState::ptr		state;

	/**
	 * add type to support network asynchronous invoke
	 */

	Task(const handler_type& h) : handler(h) {}
};

typedef list<Task>							task_list_type;
}

}}

namespace zertcore { namespace concurrent{
typedef details::Task						task_type;
}}


#endif /* SIGNAL_H_ */
