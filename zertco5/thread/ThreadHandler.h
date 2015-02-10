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
template <typename HANDLER>
class ThreadHandler
{
public:
	typedef ThreadHandler<HANDLER>			self;

public:
	typedef HANDLER							type;
	typedef utils::Params<HANDLER>			params_type;
	typedef SMART_PTR(params_type)			params_ptr;
	/**
	typedef typename result_of<HANDLER>::type
											result_type;
	*/
	typedef function<HANDLER>				function_type;

public:
	typedef thread_ids_flag_type			flag_type;

public:
	ThreadHandler();

	template <typename CALLABLE>
	ThreadHandler(const CALLABLE& func);
//	explicit ThreadHandler(const self& th);

public:
	void operator() () const {
		if (params_)
			callWithParams(function_, *params_);
		else
			function_();
	}

public:
	operator bool() const {
		return callable();
	}

	bool callable() const {
		if (params_type::args > 0 && !params_)
			return false;

		return function_? true: false;
	}

public:
	void setParams(const params_type& params);

/**
public:
	params_type& params() {return params_;}
	const params_type& params() const {return params_;}
*/
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
	mutable params_ptr			params_;

private:
	flag_type					thread_flags_;
};

}}

#endif /* THREADHANDLER_H_ */
