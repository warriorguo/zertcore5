/*
 * Runtime.h
 *
 *  Created on: 2014��9��9��
 *      Author: Administrator
 */

#ifndef ZERTCORE_RUNTIME_H_
#define ZERTCORE_RUNTIME_H_

#include <pch.h>
#include <utils/time/TimeType.h>
#include <utils/Singleton.h>

#include <concurrent/rpc/config.h>

namespace zertcore{ namespace core{
using namespace zertcore::utils;
using namespace zertcore::concurrent;
}}

namespace zertcore{ namespace core{

class Runtime :
		public Singleton<Runtime, NoneChecker>
{
public:
	typedef function<void ()>				handler_type;
	typedef list<handler_type>				handler_list_type;
	typedef multimap<time_type::type, handler_type>
											updater_list_type;

	typedef function<bool ()>				init_handler_type;
	typedef list<init_handler_type>			init_handler_list_type;

	typedef function<u32 ()>				persistent_handler_type;
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
	bool globalInit(const handler_type& init_handler = handler_type());
	void afterInitedHandler(const handler_type& init_handler);

public:
	bool initExtence();

public:
	bool setupRouter(const rpc::RPCRouterConfig& config);

public:
	void startRun();
	void run();
	void stop();

public:
	template <typename H, typename HOST>
	persistent_updater_key_type addUpdater(H handler, HOST* pthis) {
		return persistent_updater_list_.insert(persistent_updater_list_.end(),
				bind(handler, pthis));
	}
	void removeUpdater(persistent_updater_key_type key) {
		persistent_updater_list_.erase(key);
	}

private:
	u32 update();
	size_t mainThread();

private:
	void afterAllInited();

private:
	updater_list_type			updater_list_;
	persistent_updater_list_type
								persistent_updater_list_;
	init_handler_list_type		init_handler_list_;
	handler_list_type			after_inited_handler_list_;

private:
	rpc::RPCRouterConfig		router_config_;

private:
	handler_type				init_handler_;

private:
	bool						is_running_;
	time_type					now_;
};

#define RT						(::zertcore::core::Runtime::Instance())

}}

#endif /* RUNTIME_H_ */
