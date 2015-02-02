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

#include <concurrent/config.h>

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
	typedef thread_ids_flag_type			flag_type;

public:
	explicit ThreadHandler(const params_type& params);
	explicit ThreadHandler(const function_type& func, const params_type& params);

public:
	void operator() () const {
		params_.invokeWith(function_);
	}

public:
	operator bool() const {
		return function_? true: false;
	}

public:
	params_type& params() {return params_;}
	const params_type& params() const {return params_;}

public:
//	ThreadHandler& operator =(const ThreadHandler& handler);
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
	/**
	 * this method just push the request to the thread pool.
	 * it would return immediately which DID NOT meant the request had executed!
	 *
	 * to execute call operator()
	 */
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
