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
	typename PARAMS = utils::default_params_type>
class ThreadHandler
{
public:
	typedef ThreadHandler<HANDLER, PARAMS>	self;

public:
	typedef HANDLER							type;
	typedef PARAMS							params_type;
	/**
	typedef typename result_of<HANDLER>::type
											result_type;
	*/
	typedef function<HANDLER>				function_type;

public:
	typedef thread_ids_flag_type			flag_type;

public:
	ThreadHandler();
	ThreadHandler(const function_type& func);
	ThreadHandler(const params_type& params);
	ThreadHandler(const function_type& func, const params_type& params);
//	explicit ThreadHandler(const self& th);

public:
	void operator() () const {
		callWithParams(function_, params_);
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
	mutable params_type			params_;

private:
	flag_type					thread_flags_;
};

}}

#endif /* THREADHANDLER_H_ */
