/*
 * Signal.h
 *
 *  Created on: 2014��10��13��
 *      Author: Administrator
 */

#ifndef ZERTCORE_CONCURRENTSTATE_H_
#define ZERTCORE_CONCURRENTSTATE_H_

#include <pch.h>

#include <utils/types.h>
#include <utils/Singleton.h>
#include <utils/time/Timer.h>

#include <object/PoolObject.h>
#include <thread/ThreadHandler.h>

#include <core/RuntimeContext.h>

#include "coroutine/config.h"
#include "config.h"

namespace zertcore { namespace concurrent {
using namespace zertcore::utils;
using namespace zertcore::object;
}}

namespace zertcore { namespace concurrent {

typedef ThreadHandler<void (RuntimeContext::ptr)>
											cbt_handler_type;
typedef cbt_handler_type::function_type		callback_type;
typedef function<void ()>					handler_type;

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
	typedef timer_type::counter_type		tick_type;

public:
	ConcurrentState();
	ConcurrentState(const callback_type& handler);
#ifdef ZC_ENABLE_THREADHANDLER_MULTICAST
	ConcurrentState(const callback_type& handler, const tid_type& tid);
#endif
	ConcurrentState(const callback_type& handler, const thread_ids_flag_type& tids);

	virtual ~ConcurrentState();

public:
	/**
	 * set callback when all complete
	 */
	bool onComplete(const callback_type& handler);
	static void init();

public:
	operator bool() const;

public:
	bool complete(const Error& e = error::NONE);
	bool listen();

	tid_type getTid() const {return tid_;}

public:
	void setTimeout(const tick_type& intval);
private:
	void onTimeout(const tick_type& intval);

public:
	bool isError() const;
private:
	void setError(const Error& error);

#ifndef ZC_DISABLE_COROUTINE

public:
	/**
	 * coroutine support
	 */
	void setCoroutineCtx(cofcell_type::ptr co);
	cofcell_type::ptr getCoroutineCtx() {return pause_ctx_;}
	void releaseCoroutineCtx();

	void yeild();
	void resume();

#endif

public:
	ConcurrentState& operator= (const callback_type& handler);

public:
	RuntimeContext::ptr getRuntimeContext() {
		return context_;
	}

public:
	u32 getTarget() const {return target_;}
	u32 getCurrent() const {return current_;}

/**
private:
	void handleCallback();
*/
private:
	volatile u32				current_;
	volatile u32				target_;

	tid_type					tid_;
	cofcell_type::ptr			pause_ctx_;
	ptr							save_me_;

	cbt_handler_type			cb_handler_;
	RuntimeContext::ptr			context_;

	ZC_TO_STRING(
		"current:" << (uint)current_ <<
		"target:" << target_
	);

private:
	timer_type::ptr				timer_;
	spinlock_type				lock_;
};

namespace details {
/**
 * Task
 */
struct Task
{
	handler_type				handler;
	thread_ids_flag_type		flags;
	ConcurrentState::ptr		state;

	/**
	 * add type to support network asynchronous invoke
	 */

	Task(const handler_type& h, const thread_ids_flag_type& f) : handler(h), flags(f) {}
	Task() {}
};

typedef list<Task>							task_list_type;
}

}}

namespace zertcore { namespace concurrent{
typedef details::Task						task_type;
}}


#endif /* SIGNAL_H_ */
