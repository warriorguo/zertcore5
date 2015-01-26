/*
 * ThreadHandler.h
 *
 *  Created on: 2014Äê12ÔÂ22ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_THREADHANDLER_H_
#define ZERTCORE_THREADHANDLER_H_

#include <pch.h>
#include <utils/types.h>

namespace zertcore { namespace concurrent {

/**
 * ThreadHandler<HANDLER>
 */

template <typename HANDLER>
class ThreadHandler
{
public:
	typedef HANDLER							type;
	/**
	typedef typename result_of<HANDLER>::type
											result_type;
	*/
	typedef function<HANDLER>				function_type;

public:
	typedef dynamic_bitset<>				flag_type;

public:
	explicit ThreadHandler();
	explicit ThreadHandler(const function_type& func);

public:
	void operator() () const {
		function_();
	}
#ifdef ZC_COMPILE
# include "details/ThreadHandlerOperator.ipp"
#endif

public:
	operator bool() const {
		return function_? true: false;
	}

public:
	ThreadHandler& operator =(const ThreadHandler& handler);
	ThreadHandler& operator =(const function_type& handler);

public:
	void setThreadIndex(const __ALL& _);
	void setThreadIndex(const __OTHER& _);
	void setThreadIndex(const flag_type& flags);
	bool setThreadIndex(const tid_type& index);

	const flag_type& getThreadIndex() const {
		return thread_flags_;
	}

	void lazyThreadIndex(const u32& index);

public:
#ifdef ZC_COMPILE
# include "details/ThreadHandlerSetParams.ipp"
#else
	template <typename NEW_HANDLER, typename T1>
	ThreadHandler<NEW_HANDLER> setParams(T1 p1) const {
		ThreadHandler<NEW_HANDLER> th(bind(function_, p1));
		th.setThreadIndex(thread_flags_);

		return th;
	}
	template <typename NEW_HANDLER, typename T1, typename T2>
	ThreadHandler<NEW_HANDLER> setParams(T1 p1, T2 p2) const {
		ThreadHandler<NEW_HANDLER> th(bind(function_, p1, p2));
		th.setThreadIndex(thread_flags_);

		return th;
	}
	template <typename NEW_HANDLER, typename T1, typename T2, typename T3>
	ThreadHandler<NEW_HANDLER> setParams(T1 p1, T2 p2, T3 p3) const {
		ThreadHandler<NEW_HANDLER> th(bind(function_, p1, p2, p3));
		th.setThreadIndex(thread_flags_);

		return th;
	}
#endif

public:
	bool push() const;
	void clear() {
		function_ = function_type();
		thread_flags_.reset();
	}

private:
	function_type				function_;

private:
	flag_type					thread_flags_;
};


}}

#endif /* THREADHANDLER_H_ */
