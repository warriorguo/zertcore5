/*
 * RPC.h
 *
 *  Created on: 2015年5月13日
 *      Author: Administrator
 */

#ifndef ZERTCORE_CONCURRENT_RPC_RPC_H_
#define ZERTCORE_CONCURRENT_RPC_RPC_H_

#include "RPCManager.h"

namespace zertcore { namespace concurrent { namespace rpc {

/**
 * RPCUser, a wrapper of RPCManager,
 * just add a key checker to avoid invoke/register sys key
 */
class RPCUser : public RPCManager
{
public:
	bool registerRPCHandler(const key_type& key,
			const rpc_handler_type& handler) {
		if (RPCSpec::isSystemKey(key))
			return false;
		return RPCManager::registerRPCHandler(key, handler);
	}
	bool registerDataSyncHandler(const key_type& key,
			const data_sync_handler_type& handler) {
		if (RPCSpec::isSystemKey(key))
			return false;
		return RPCManager::registerDataSyncHandler(key, handler);
	}

public:
	//make data sync
	bool notify(const key_type& key, const iarchiver_type& values) {
		if (RPCSpec::isSystemKey(key))
			return false;
		return RPCManager::notify(key, values);
	}

public:
	/**
	 * key, to the remote host register method's key
	 * params
	 * handler
	 * state concurrent state
	 * id (with the default fetcher), if same id (not 0), the id would try to send to the same host at the first time,
	 *     but if that host was not available, would try other host
	 * fetcher would fetch the host(s) that would be invoked
	 */
	bool asyncCall(const key_type& key, const iarchiver_type& params,
			const rpc_callback_type& handler = rpc_callback_type(),
			ConcurrentState::ptr state = ConcurrentState::ptr(),
			const uuid_t& id = 0, const RPCCallFetcher& fetcher = default_fetcher_type(),
			const tick_type& timeout = 10000) {
		if (RPCSpec::isSystemKey(key))
			return false;

		return RPCManager::asyncCall(key, params, handler, state, id, fetcher, timeout);
	}

	bool call(const key_type& key, const iarchiver_type& params,
			oarchiver_type& oar, const uuid_t& id = 0,
			const RPCCallFetcher& fetcher = default_fetcher_type(),
			const tick_type& timeout = 10000) {
		if (RPCSpec::isSystemKey(key))
			return false;

		return RPCManager::call(key, params, oar, id, fetcher, timeout);
	}
};

}}}

#define RPC						(::zertcore::concurrent::rpc::RPCManager::Instance())

#endif /* CONCURRENT_RPC_RPC_H_ */
