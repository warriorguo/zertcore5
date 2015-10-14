/*
 * RPCManager.cpp
 *
 *  Created on: 2015年2月1日
 *      Author: Administrator
 */

#include <concurrent/Concurrent.h>

#include "RPCManager.h"
#include "RPCSpec.h"
#include "RPCRouterClient.h"

#include <details.h>

namespace zertcore { namespace concurrent { namespace rpc {

// RPCManager::RPCManager() {}

bool RPCManager::
setup(const RemoteConfig& server_config,
		const RemoteConfig& client_config) {
	return 	RPCSpec::initServer(server_config) &&
			RPCSpec::initClient(client_config) &&
			RPCSpec::setup();
}

bool RPCManager::
connectRouter(const RPCRouterConfig& router_config) {
	return RPCSpec::router().init(router_config);
}

void RPCManager::
setRPCReadyHandler(const RPCRouterClient::on_handler_type& handler) {
	RPCSpec::router().setRPCReadyHandler(handler);
}

bool RPCManager::
registerRPCHandler(const key_type& key, const rpc_handler_type& handler) {
	return RPCSpec::registerRPCHandler(key, handler);
}

bool RPCManager::
registerDataSyncHandler(const key_type& key, const data_sync_handler_type& handler,
		const condition_group& expr) {
	return RPCSpec::registerDataSyncHandler(key, handler, expr);
}

void RPCManager::
finishRegister(ConcurrentState::ptr state) {
	RPCSpec::syncRouter(state);
}

bool RPCManager::
notify(const key_type& key, const iarchiver_type& params) {
	Group<RPCClientConnection::ptr> conns = RPCSpec::router().getClientConnections(key, params);
	if (conns.empty())
		return false;

	bool ret = false;
	conns.foreach([&](RPCClientConnection::ptr conn) {
		ret = RPCSpec::Instance().makeNotify(conn, key, params) && ret;
	});

	return ret;
}

bool RPCManager::
asyncCall(const key_type& key, const iarchiver_type& params,
		const rpc_callback_type& handler, ConcurrentState::ptr state, const uuid_t& id,
		const RPCCallFetcher& fetcher, const tick_type& timeout_ms) {
	Group<RPCClientConnection::ptr> conns = RPCSpec::router().getServerConnection(key);

	bool ret = true;
	if (!fetcher.fetch(conns, id, [&] (RPCClientConnection::ptr conn) {
		ret = RPCSpec::Instance().makeAsyncCall(conn, key, params, handler, state, timeout_ms) && ret;
	}))
		return false;

	return ret;
}

bool RPCManager::
call(const key_type& key, const iarchiver_type& params,
		oarchiver_type& oar, const uuid_t& id,
		const RPCCallFetcher& fetcher, const tick_type& timeout_ms) {
	Group<RPCClientConnection::ptr> conns = RPCSpec::router().getServerConnection(key);

	bool ret = true;
	if (!fetcher.fetch(conns, id, [&] (RPCClientConnection::ptr conn) {
		ret = RPCSpec::Instance().makeCall(conn, key, params, oar, timeout_ms) && ret;
	}))
		return false;

	return ret;
}

}}}
