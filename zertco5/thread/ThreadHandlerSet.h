/*
 * ThreadHandlerSet.h
 *
 *  Created on: 2015年5月22日
 *      Author: Administrator
 */

#ifndef ZERTCORE_THREAD_THREADHANDLERSET_H_
#define ZERTCORE_THREAD_THREADHANDLERSET_H_

#include <pch.h>
#include <utils/types.h>

#include "config.h"

namespace zertcore { namespace concurrent {

/**
 * ThreadHandlerSet
 */
template <class Final>
class ThreadHandlerSet
{
public:
	enum {
		THREAD_INDEX						= 1,
	};

public:
	virtual ~ThreadHandlerSet() {}

public:
	static tid_type getThreadIndex();

public:
	void initHandler(const init_handler_type& handler);

	template <typename Callable>
	static bool go(Callable caller);
/**
public:
	template <typename Callable>
	void push(Callable caller) {
		_push(is_member_function_pointer<Callable>(), caller);
	}

private:
	template <typename Callable>
	void _push(const true_type& is_member_func, Callable caller) {
		typedef decltype((this->*caller)())	r_type;

		ThreadHandler<r_type ()> h(bind(caller, __getChildThis()));
		h.setThreadIndex(getThreadIndex());
		h.push();
	}
	template <typename Callable>
	void _push(const false_type& is_not_member_func, Callable caller) {
		typedef decltype((*caller)())		r_type;

		ThreadHandler<r_type ()> h(caller);
		h.setThreadIndex(getThreadIndex());
		h.push();
	}
#ifndef ZC_COMPILE
public:
	template <typename Callable, typename ...TN>
	void push(const Callable& caller, TN... Tn) {
		_push(is_member_function_pointer<Callable>(), caller, forward<TN>(Tn)...);
	}

private:
	template <typename Callable, typename ...TN>
	void _push(const true_type& is_member_func, const Callable& caller, TN... Tn) {
		auto b = bind(caller, __getChildThis(), forward<TN>(Tn)...);

		ThreadHandler<decltype(b()) ()> h(b);
		h.setThreadIndex(getThreadIndex());
		h.push();
	}
	template <typename Callable, typename ...TN>
	void _push(const false_type& is_not_member_func, const Callable& caller, TN... Tn) {
		auto b = bind(caller, forward<TN>(Tn)...);
		ThreadHandler<decltype(b()) ()> h(b);
		h.setThreadIndex(getThreadIndex());
		h.push();
	}
#else

//#  include "details/ThreadHandlerSetPush.ipp"

#endif
*/

private:
	Final* __getChildThis() {return static_cast<Final *>(this);}
};

}}


#endif /* THREAD_THREADHANDLERSET_H_ */
