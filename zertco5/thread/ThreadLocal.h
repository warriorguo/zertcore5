/*
 * ThreadState.h
 *
 *  Created on: 2014Äê12ÔÂ22ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_THREADSTATE_H_
#define ZERTCORE_THREADSTATE_H_

#include <pch.h>

#include <utils/types.h>
#include <utils/Singleton.h>

namespace zertcore { namespace concurrent {

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
	ThreadLocal() {
		pthread_key_create(&specific_key_, NULL);
	}
	~ThreadLocal() {
		pthread_key_delete(specific_key_);
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
	value_type& load() const {
		static value_ptr not_in_thread_value = null;
		value_ptr vp = null;
		bool in_thread_flag = false;

		if (::getpid() == ::syscall(SYS_gettid)) {
			vp = not_in_thread_value;
		}
		else {
			in_thread_flag = true;
			vp = (value_ptr)pthread_getspecific(specific_key_);
		}

		if (!vp) {
			void* buf = ::malloc(sizeof(value_type));
			memset(buf, 0, sizeof(value_type));

			vp = new (buf) value_type;

			if (init_handler_)
				init_handler_(*vp);

			if (in_thread_flag) {
				pthread_setspecific(specific_key_, vp);
			}
			else {
				not_in_thread_value = vp;
			}
		}

		return *vp;
	}

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
