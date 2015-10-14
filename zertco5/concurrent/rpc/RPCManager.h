/*
 * RPCManager.h
 *
 *  Created on: 2015年2月1日
 *      Author: Administrator
 */

#ifndef ZERTCORE_RPCMANAGER_H_
#define ZERTCORE_RPCMANAGER_H_

#include <utils/Singleton.h>
#include <object/PoolObject.h>

#include "config.h"
#include "RPCConnection.h"
#include "RPCClient.h"
#include "RPCSpec.h"

#include "details/RPCFetcher.h"

/**
 * for key_type:
 *  Can't start with *, these keys were reserved for system
 *
 * RFC for Request:
 *
 * reqs["id"] -> u32 MessageID unique ID for a request
 * reqs["key"] -> key_type
 * reqs["params"] -> Object
 *
 * for Response:
 * ret["head"]["id"] -> u32
 * optional ret["head"]["err"] -> string
 * optional ret["body"][key1] -> Object1
 * 			ret["body"][key2] -> Object2
 *
 */

namespace zertcore { namespace concurrent { namespace rpc {
using namespace zertcore::utils;
using namespace zertcore::object;

typedef RPCDefaultFetcher					default_fetcher_type;

}}}

namespace zertcore { namespace concurrent { namespace rpc {

/**
 * Interface for RPC
 */
class RPCManager :
		public Singleton<RPCManager>
{
public:
	/**
	 * 2 ports need to bind in the RPC framework,
	 * 1 for RPC Service, the other for the data sync
	 */
	static bool setup(const RemoteConfig& server_config, const RemoteConfig& client_config);
	bool connectRouter(const RPCRouterConfig& router_config);
	void setRPCReadyHandler(const RPCRouterClient::on_handler_type& handler = RPCRouterClient::on_handler_type());

public:
	/**
	 *
	 */
	bool registerRPCHandler(const key_type& key, const rpc_handler_type& handler);
	bool registerDataSyncHandler(const key_type& key, const data_sync_handler_type& handler,
			const condition_group& expr = condition_group());
	/**
	 * this method called in other thread
	 */
	void finishRegister(ConcurrentState::ptr state = ConcurrentState::ptr());

public:
	//make data sync
	bool notify(const key_type& key, const iarchiver_type& data);

public:
	/**
	 * key, to the remote host register method's key
	 * params
	 * handler
	 * state concurrent state
	 * id (with the default fetcher), if same id (not 0),
	 *     the id would try to send to the same host at the first time,
	 *     but if that host was not available, would try other fastest host
	 * fetcher would fetch the host(s) that would be invoked
	 */
	bool asyncCall(const key_type& key, const iarchiver_type& params,
			const rpc_callback_type& handler = rpc_callback_type(),
			ConcurrentState::ptr state = ConcurrentState::ptr(),
			const uuid_t& id = 0, const RPCCallFetcher& fetcher = default_fetcher_type(),
			const tick_type& timeout_ms = 10000);

	bool call(const key_type& key, const iarchiver_type&,
			oarchiver_type& oar, const uuid_t& id = 0,
			const RPCCallFetcher& fetcher = default_fetcher_type(), const tick_type& timeout_ms = 10000);

};

}}}

#endif /* RPCMANAGER_H_ */
