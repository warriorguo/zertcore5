/*
 * RPCRouterManager.h
 *
 *  Created on: 2015年2月9日
 *      Author: Administrator
 */

#ifndef ZERTCORE_RPCROUTERMANAGER_H_
#define ZERTCORE_RPCROUTERMANAGER_H_

#include "config.h"
#include "RPCConnection.h"

namespace zertcore { namespace concurrent { namespace rpc {

/**
 * ConnectionConfig
 */
struct ClientConnectionConfig
{
	RemoteConfig				rc;
	RPCClientConnection::ptr	conn;
};
struct ServerConnectionConfig
{
	RemoteConfig				rc;
	RPCServerConnection::ptr	conn;
};

typedef unordered_map<key_type, ClientConnectionConfig>
											key_client_connection_map_type;
typedef unordered_map<key_type, ServerConnectionConfig>
											key_server_connection_map_type;
typedef unordered_map<string, RPCServerConfig>
											rpc_server_map_type;
typedef unordered_map<string, RPCClientConfig>
											rpc_client_map_type;

/**
 * RPCRouterManager
 */
class RPCRouterManager
{
public:
	enum {
		FLAG_NONE							= 0,
		FLAG_SERVER							= 1 << 0,
		FLAG_CLIENT							= 1 << 1,
	};

public:
	RPCRouterManager() : update_config_flag_(FLAG_NONE) {}

public:
	bool connect();

public:
	/**
	 * Sync to route server
	 */
	void registerRPCHandlers(const key_type& key) {
		server_config_.call_keys.insert(key);
		update_config_flag_ |= FLAG_SERVER;
	}
	void registerDataSyncHandlers(const key_type& key) {
		client_config_.keys.insert(key);
		update_config_flag_ |= FLAG_CLIENT;
	}

	/**
	 * Sync data with router server based on update_config_flag_
	 */
	void sendRouter();

public:
	/**
	 * if ConnectionConfig.conn was null, make it work
	 */
	RPCServerConnection::ptr getServerConnection(const key_type& key);
	RPCClientConnection::ptr getClientConnection(const key_type& key);

private:
	/**
	 * Got data of a RPCServerConfig
	 * remove all existed data from the server config
	 * add all the keys to key_server_map_
	 */
	void syncServerConfig(const key_type&, const oachiver_type& params);
	/**
	 * Got data of a RPCClientConfig
	 * remove all existed data from the client config
	 * add all the keys to key_client_map_
	 */
	void syncClientConfig(const key_type&, const oachiver_type& params);

private:
	/**
	 * Local config
	 */
	u32							update_config_flag_;
	RPCServerConfig				server_config_;
	RPCClientConfig				client_config_;

private:
	/**
	 * Remote config
	 */
	key_server_connection_map_type
								key_server_conn_map_;
	key_client_connection_map_type
								key_client_conn_map_;

	rpc_server_map_type			rpc_server_map_;
	rpc_client_map_type			rpc_client_map_;
};

}}}


#endif /* RPCROUTERMANAGER_H_ */
