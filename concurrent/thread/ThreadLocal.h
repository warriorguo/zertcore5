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
	void setInitHandler(const value_handler_type& handler) {
		init_handler_ = handler;
	}
	void setDeinitHandler(const value_handler_type& handler) {
		deinit_handler_ = handler;
	}

	/**
	 * Called in threads
	 */
	value_type& load() const {
		value_ptr vp = (value_ptr)pthread_getspecific(specific_key_);
		if (!vp) {
			vp = new value_type;

			if (init_handler_)
				init_handler_(*vp);

			pthread_setspecific(specific_key_, vp);
		}

		return *vp;
	}

private:
	pthread_key_t				specific_key_;
	value_handler_type			init_handler_,
								deinit_handler_;
};

}}


#endif /* THREADSTATE_H_ */
