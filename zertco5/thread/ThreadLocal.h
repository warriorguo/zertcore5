/*
 * ThreadState.h
 *
 *  Created on: 2014��12��22��
 *      Author: Administrator
 */

#ifndef ZERTCORE_THREADSTATE_H_
#define ZERTCORE_THREADSTATE_H_

#include <pch.h>
#include <utils/types.h>

namespace zertcore { namespace concurrent {

namespace helper {

static inline bool isInThread() {
	return ::getpid() != ::syscall(SYS_gettid);
}

}
using namespace helper;

#define ZC_DISABLE_NOT_IN_THREAD_VALUE

/**
 * ThreadVariables
 */
template <typename T>
struct ThreadLocal
{
public:
	typedef T								value_type;
	typedef T*								value_ptr;

public:
	typedef function<void (value_type&)>	value_handler_type;

public:
	ThreadLocal()
#ifndef ZC_DISABLE_NOT_IN_THREAD_VALUE
: not_in_thread_value_(null)
#endif
{
		pthread_key_create(&specific_key_, NULL);
	}
	~ThreadLocal() {
		pthread_key_delete(specific_key_);

#ifndef ZC_DISABLE_NOT_IN_THREAD_VALUE
		if (not_in_thread_value_)
			delete not_in_thread_value_;
#endif
	}

public:
	static void setInitHandler(const value_handler_type& handler) {
		init_handler_ = handler;
	}
	static void setDeinitHandler(const value_handler_type& handler) {
		deinit_handler_ = handler;
	}

public:
	void deinit();
	/**
	 * Called in threads
	 */
	inline value_type& load() const {
#ifndef ZC_DISABLE_NOT_IN_THREAD_VALUE
		if (!isInThread()) {
			return loadByMainThread();
		}
#endif

		value_ptr vp = (value_ptr)pthread_getspecific(specific_key_);

		if (!vp) {
			void* buf = ::calloc(sizeof(value_type), 1);
			vp = new (buf) value_type;

			if (init_handler_)
				init_handler_(*vp);

			pthread_setspecific(specific_key_, vp);
		}

		return *vp;
	}

#ifndef ZC_DISABLE_NOT_IN_THREAD_VALUE

private:
	value_type& loadByMainThread() const {
		if (not_in_thread_value_)
			return *not_in_thread_value_;

		void* buf = ::calloc(sizeof(value_type), 1);
		not_in_thread_value_ = new (buf) value_type;

		if (init_handler_)
			init_handler_(*not_in_thread_value_);

		return *not_in_thread_value_;
	}


private:
	mutable value_ptr			not_in_thread_value_;

#endif

private:
	pthread_key_t				specific_key_;

private:
	static value_handler_type	init_handler_,
								deinit_handler_;
};

template <typename T>
typename ThreadLocal<T>::value_handler_type
								ThreadLocal<T>::init_handler_;
template <typename T>
typename ThreadLocal<T>::value_handler_type
								ThreadLocal<T>::deinit_handler_;

}}


#endif /* THREADSTATE_H_ */
