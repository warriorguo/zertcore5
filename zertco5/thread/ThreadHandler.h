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

#ifdef ZC_ENABLE_THREADHANDLER_STATE

#include <object/ObjectTraits.h>

namespace zertcore { namespace concurrent {
using namespace zertcore::object;

class ConcurrentState;
typedef ObjectTraits<ConcurrentState>::ptr	current_state_ptr;
}}

#endif

namespace zertcore { namespace concurrent {

/**
 * ThreadHandler<HANDLER>
 */
template <typename HANDLER>
class ThreadHandler
{
public:
	typedef ThreadHandler<HANDLER>			self;

#ifdef ZC_ENABLE_THREADHANDLER_STATE
	typedef list<current_state_ptr>			current_state_list_type;
#endif

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

#ifdef ZC_ENABLE_THREADHANDLER_MULTICAST
	template <typename CALLABLE>
	ThreadHandler(const CALLABLE& func, const tid_type& tid);
#endif

	template <typename CALLABLE>
	ThreadHandler(const CALLABLE& func, const flag_type& tids);

public:
	void operator() () const;

public:
	operator bool() const {
		return function_? true: false;
	}

	bool callable() const {
		if (params_type::args > 0 && !params_)
			return false;

		return function_? true: false;
	}

public:

#ifndef ZC_ENABLE_PARAMS_REF

	template <typename T0>
	void setParams(T0 t0) const {
		params_ = params_ptr(new params_type(t0));
	}

	template <typename T0,typename T1>
	void setParams(T0 t0, T1 t1) const {
		params_ = params_ptr(new params_type(t0, t1));
	}

	template <typename T0,typename T1,typename T2>
	void setParams(T0 t0, T1 t1, T2 t2) const {
		params_ = params_ptr(new params_type(t0, t1, t2));
	}

	template <typename T0,typename T1,typename T2,typename T3>
	void setParams(T0 t0, T1 t1, T2 t2, T3 t3) const {
		params_ = params_ptr(new params_type(t0, t1, t2, t3));
	}

	template <typename T0,typename T1,typename T2,typename T3,typename T4>
	void setParams(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4) const {
		params_ = params_ptr(new params_type(t0, t1, t2, t3, t4));
	}

#else

#  include "details/ThreadHandlerSetParams.ipp"

#endif

/**
public:
	params_type& params() {return params_;}
	const params_type& params() const {return params_;}
*/
public:
	ThreadHandler& operator =(const ThreadHandler& handler);
	ThreadHandler& operator =(const function_type& handler);

public:
	void setThreadIndex(const __ALL& _);
	void setThreadIndex(const __OTHER& _);
#ifdef ZC_ENABLE_THREADHANDLER_MULTICAST
	void setThreadIndex(const flag_type& flags);
#endif
	bool setThreadIndex(const tid_type& index);

	const flag_type& getThreadIndex() const {
		return thread_flags_;
	}

	/**
	 * this Method GUARANTEE the same index would make the handler
	 * execute in the same thread.
	 *
	 * for the parameter @index, it donesnt need make sure in the valid range of thread ids.
	 *
	 * But IMPORTANT, different @index may run in the same thread!
	 */
	void lazyThreadIndex(const u32& index);

public:
	void setPriority(const priority_type& pri) {priority_ = pri;}
	priority_type getPriority() const {return priority_;}

public:
	/**
	 * this method just push the request to the thread pool.
	 * it would return immediately which DID NOT meant the request had executed!
	 *
	 * to execute call operator()
	 */
	bool push() const;
	bool push(const params_type& params) const;
	void clear() {
		function_ = function_type();
#ifdef ZC_ENABLE_THREADHANDLER_MULTICAST
		thread_flags_.reset();
#else
		thread_flags_ = 0;
#endif
	}

#ifdef ZC_ENABLE_THREADHANDLER_STATE
public:
	void setupState(current_state_ptr state);

private:
	void completeState() const;

private:
	current_state_list_type		current_state_list_;

#endif

private:
	function_type				function_;
	mutable params_ptr			params_;

private:
	flag_type					thread_flags_;
	priority_type				priority_;
};

}}

namespace zertcore {

template <typename Callable>
inline bool goThread(tid_type tid, Callable caller) {
	concurrent::ThreadHandler<void ()> handler(caller);
	if (!handler.setThreadIndex(tid))
		return false;

	return handler.push();
}

}

#endif /* THREADHANDLER_H_ */
