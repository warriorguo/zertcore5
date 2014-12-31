/*
 * RunnableHandler.h
 *
 *  Created on: 2014Äê12ÔÂ1ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_THREADABLEHANDLER_H_
#define ZERTCORE_THREADABLEHANDLER_H_

#include <pch.h>
#include <utils/types.h>

#include <concurrent/config.h>

namespace zertcore { namespace concurrent {

/**
 * ThreadableHandler
 */
struct ThreadableHandler
{
	typedef function<void (RunningContext&)>
								origin_handler_type;

	origin_handler_type			handler;
	RunningContext				context;
	thread_index_type			thread_index;

	ThreadableHandler(const origin_handler_type& h) : handler(h) {}

	operator bool() const {
		return handler? true: false;
	}

	ThreadableHandler& operator = (const origin_handler_type& h);

	/**
	 * inhert
	 */
	bool inhert(const ThreadableHandler& handler);

	/**
	 * pushThread
	 */
	bool pushThread(const thread_index_type& ti) {
		thread_index = ti;
		return pushThread();
	}
	bool pushThread();

public:
	bool operator() () {
		if (context) {
			return false;
		}
		if (handler) {
			handler(context);
			return true;
		}

		return false;
	}

private:
	detail::spinlock			lock_;
};

}}


#endif /* RUNNABLEHANDLER_H_ */
