/*
 * Runtime.h
 *
 *  Created on: 2014Äê9ÔÂ9ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_RUNTIME_H_
#define ZERTCORE_RUNTIME_H_

#include <pch.h>
#include <utils/time/TimeType.h>
#include <utils/Singleton.h>

namespace zertcore{ namespace core{
using namespace zertcore::utils;
}}

namespace zertcore{ namespace core{

class Runtime :
		public Singleton<Runtime>
{
public:
	typedef function<void ()>				handler_type;
	typedef multimap<time_type::type, handler_type>
											updater_list_type;

	typedef function<uint ()>				persistent_handler_type;
	typedef list<persistent_handler_type>	persistent_updater_list_type;

public:
	typedef updater_list_type::iterator		updater_key_type;
	typedef persistent_updater_list_type::iterator
											persistent_updater_key_type;

public:
	Runtime() : is_running_(false) {}

public:
	/**
	 * Setup thread pool here
	 */
	bool init(const handler_type& init_handler = handler_type());

public:
	void startRun();
	void run();

public:
	template <typename H, typename HOST>
	persistent_updater_key_type addUpdater(H handler, HOST* pthis) {
		return persistent_updater_list_.insert(persistent_updater_list_.end(),
				bind(handler, pthis));
	}
	void removeUpdater(persistent_updater_key_type key) {
		persistent_updater_list_.erase(key);
	}

public:
	updater_key_type setTimeout(time_type interval, handler_type handler) {
		return updater_list_.insert(updater_list_type::value_type(interval.value,
				handler));
	}
	template<typename H, typename HOST>
	updater_key_type setTimeout(time_type interval, H handler, HOST* pthis) {
		return setTimeout(interval, bind(&handler, pthis));
	}
	template<typename H, typename HOST, typename T1>
	updater_key_type setTimeout(time_type interval, H handler, HOST* pthis,
			T1 p1) {
		return setTimeout(interval, bind(&handler, pthis, p1));
	}
	template<typename H, typename HOST, typename T1, typename T2>
	updater_key_type setTimeout(time_type interval, H handler, HOST* pthis,
			T1 p1, T1 p2) {
		return setTimeout(interval, bind(&handler, pthis, p1, p2));
	}
	template<typename H, typename HOST, typename T1, typename T2, typename T3>
	updater_key_type setTimeout(time_type interval, H handler, HOST* pthis,
			T1 p1, T1 p2, T1 p3) {
		return setTimeout(interval, bind(&handler, pthis, p1, p2, p3));
	}

private:
	u32 update();
	void mainThread();

private:
	updater_list_type			updater_list_;
	persistent_updater_list_type
								persistent_updater_list_;

private:
	handler_type				init_handler_;

private:
	bool						is_running_;
	time_type					now_;
};

#define RT						(::zertcore::core::Runtime::Instance())

}}

#include <utils/time/details/TimeTypeSerialization.hpp>

#endif /* RUNTIME_H_ */
