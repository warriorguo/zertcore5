/*
 * ThreadHandler.h
 *
 *  Created on: 2014��12��22��
 *      Author: Administrator
 */

#ifndef ZERTCORE_THREADHANDLER_H_
#define ZERTCORE_THREADHANDLER_H_

#include <pch.h>
#include <utils/types.h>
#include <utils/Params.h>

namespace zertcore { namespace concurrent {

/**
 * ThreadHandler<HANDLER>
 */
template <typename HANDLER,
	typename BIND_PARAM1 = void,
	typename BIND_PARAM2 = void,
	typename BIND_PARAM3 = void,
	typename BIND_PARAM4 = void,
	typename BIND_PARAM5 = void>
class ThreadHandler
{
public:
	typedef HANDLER							type;
	typedef utils::Params<BIND_PARAM1,
			BIND_PARAM2, BIND_PARAM3,
			BIND_PARAM4, BIND_PARAM5>		params_type;
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
		params_.invokeWith(function_);
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

	/**
	 * this Method GUARANTEE same index would make the handler
	 * execute in the same thread.
	 *
	 * for the parameter @index, it donesnt need make sure in the valid range of thread ids.
	 *
	 * But IMPORTANT, different @index may run in the same thread!
	 */
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
	params_type					params_;

private:
	flag_type					thread_flags_;
};

}}

#endif /* THREADHANDLER_H_ */
