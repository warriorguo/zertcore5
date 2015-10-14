/*
 * RPCRouterManager.cpp
 *
 *  Created on: 2015年2月12日
 *      Author: Administrator
 */

#include <log/Log.h>
#include <concurrent/ConcurrentState.h>

#include "RPCRouterClient.h"
#include "RPCClient.h"
#include "RPCServer.h"
#include "RPCSpec.h"

#include <details.h>

namespace zertcore { namespace concurrent { namespace rpc {

bool RPCRouterClient::
init(const RPCRouterConfig& config) {
	router_conn_ = RPCClient::Instance().connect(config);
	if (!router_conn_) {
		ZCLOG(ERROR) << "Connect to " << config.toString()
				<< " failed" << End;
		return false;
	}

	if (!RPCSpec::Instance().registerDataSyncHandler(cmd::REG_RPC_SYNC_SERVER,
			bind(&RPCRouterClient::syncServerConfig, this, _1, _2))) {
		ZCLOG(ERROR) << "registerDataSyncHandler(cmd::REG_RPC_SYNC_SERVER) failed" << End;
		return false;
	}
	if (!RPCSpec::Instance().registerDataSyncHandler(cmd::REG_RPC_SYNC_CLIENT,
			bind(&RPCRouterClient::syncClientConfig, this, _1, _2))) {
		ZCLOG(ERROR) << "registerDataSyncHandler(cmd::REG_RPC_SYNC_CLIENT) failed" << End;
		return false;
	}

	return true;
}

void RPCRouterClient::
notifyClose(ConcurrentState::ptr finish_state) {
	ZC_ASSERT(router_conn_);

	server_config_.call_keys.clear();
	client_config_.keys.clear();

	do {
		iarchiver_type archiver;
		archiver & server_config_;

		archiver["close"] & 1;

		if (finish_state) finish_state->listen();

		RPCSpec::Instance().makeAsyncCall(router_conn_,
			cmd::REG_RPC_SYNC_SERVER, archiver,
			[finish_state] (key_type key, Error error, oarchiver_type o) {
				// check error
				ZC_ASSERT(!error);

				if (finish_state) finish_state->complete();
			});
	}
	while(false);

	do {
		iarchiver_type archiver;
		archiver & client_config_;

		archiver["close"] & 1;

		if (finish_state) finish_state->listen();

		RPCSpec::Instance().makeAsyncCall(router_conn_,
			cmd::REG_RPC_SYNC_CLIENT, archiver,
			[finish_state] (key_type key, Error error, oarchiver_type o) {
				// check error
				ZC_ASSERT(!error);

				if (finish_state) finish_state->complete();
			});
	}
	while(false);
}

void RPCRouterClient::
registerRPCHandlers(const key_type& key) {
	server_config_.call_keys.insert(key);
	update_config_flag_ |= FLAG_SERVER;
}

void RPCRouterClient::
registerDataSyncHandlers(const key_type& key, const condition_group& cond) {
	client_config_.keys[key] = cond;
	update_config_flag_ |= FLAG_CLIENT;
}

void RPCRouterClient::
notifyRouter(ConcurrentState::ptr finish_state) {
	ZC_ASSERT(router_conn_);

	if ((update_config_flag_ & FLAG_SERVER) && !server_config_.call_keys.empty()) {
		iarchiver_type archiver;
		archiver & server_config_;

		if (finish_state) finish_state->listen();

		RPCSpec::Instance().makeAsyncCall(router_conn_,
			cmd::REG_RPC_SYNC_SERVER, archiver,
			[finish_state, this] (key_type key, Error error, oarchiver_type o) {
				// check error
				ZCLOG(NOTE) << "CB for sync router server" << End;
				ZC_ASSERT(!error);

				uuid_t id = 0;
				/**
				 * the return of id could not be null
				 */
				ZC_ASSERT(o["id"] & id);

				server_config_.id = id;
				if (finish_state) finish_state->complete();
			});
	}
	if (update_config_flag_ & FLAG_CLIENT) {
		iarchiver_type archiver;
		archiver & client_config_;

		if (finish_state) finish_state->listen();

		RPCSpec::Instance().makeAsyncCall(router_conn_,
			cmd::REG_RPC_SYNC_CLIENT, archiver,
			[finish_state] (key_type key, Error error, oarchiver_type o) {
				// check error
				ZCLOG(NOTE) << "CB for sync router client" << End;
				ZC_ASSERT(!error);

				if (finish_state) finish_state->complete();
			});
	}
	else {
		ZCLOG(NOTICE) << "Nothing to do with sendRouter()!" << End;
		return ;
	}

	update_config_flag_ = FLAG_NONE;
}

Group<RPCClientConnection::ptr> RPCRouterClient::
getServerConnection(const key_type& key) {
	spinlock_guard_type guard(server_lock_);

	auto rits = key_server_conn_map_.equal_range(key);

	Group<RPCClientConnection::ptr> group;
	for (auto rit = rits.first; rit != rits.second; ++rit) {
		if (!rit->second.conn) {
			if (!rit->second.conn) {
				rit->second.conn = RPCClient::Instance().connect(rit->second.rc);
			}
		}

		if (rit->second.conn)
			group.add(rit->second.conn);
	}

	return group;
}

Group<RPCClientConnection::ptr> RPCRouterClient::
getClientConnections(const key_type& key, const iarchiver_type& iar) {
	spinlock_guard_type guard(client_lock_);

	auto rits = key_client_conn_map_.equal_range(key);
	oarchiver_type o(iar);

	Group<RPCClientConnection::ptr> group;
	for (auto rit = rits.first; rit != rits.second; ++rit) {
		if (!rit->second.conn) {
			if (!rit->second.conn) {
				rit->second.conn = RPCClient::Instance().connect(rit->second.rc);
			}
		}

		if (rit->second.conn && rit->second.cond(o))
			group.add(rit->second.conn);
	}

	return group;
}

/**
 * router would send all servers information at the time
 */
void RPCRouterClient::
syncServerConfig(key_type key, oarchiver_type params) {
	spinlock_guard_type guard(server_lock_);

	ZC_DEBUG_ASSERT(key == cmd::REG_RPC_SYNC_SERVER);

	ZCLOG(NOTE) << "syncServerConfig()" << End;

	vector<RPCServerConfig> v;
	params & v;

	rpc_server_map_.clear();
	key_server_conn_map_.clear();

	for (u32 i = 0; i < v.size(); ++i) {
		string key = v[i].rc.toString();

		rpc_server_map_[key] = v[i];

		/**
		 * bind keys to key_server_conn_map_
		 */
		ConnectionConfig config;
		config.rc = v[i].rc;

		for (auto it = v[i].call_keys.begin();
				it != v[i].call_keys.end(); ++it) {
			key_server_conn_map_.insert(
					key_server_connection_map_type::value_type(*it, config));
		}
	}

	if (on_rpc_ready_handler_)
		on_rpc_ready_handler_.push();
}

/**
 * router would send all client information to the client at the time
 */
void RPCRouterClient::
syncClientConfig(key_type key, oarchiver_type params) {
	spinlock_guard_type guard(client_lock_);
	ZC_DEBUG_ASSERT(key == cmd::REG_RPC_SYNC_CLIENT);

	ZCLOG(NOTE) << "syncClientConfig()" << End;

	vector<RPCClientConfig> v;
	params & v;

	rpc_client_map_.clear();
	key_client_conn_map_.clear();

	for (u32 i = 0; i < v.size(); ++i) {
		string key = v[i].rc.toString();

		rpc_client_map_[key] = v[i];

		/**
		 * bind keys to key_client_conn_map_
		 */
		ClientConnectionConfig config;
		config.rc = v[i].rc;

		for (auto it = v[i].keys.begin();
				it != v[i].keys.end(); ++it) {
			config.cond = it->second;

			::printf("Add %s to %s\n", it->first.c_str(), key.c_str());

			key_client_conn_map_.insert(
					key_client_connection_map_type::value_type(it->first, config));
		}
	}
}

}}}
