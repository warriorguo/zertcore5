/*
 * RPCRouterManager.h
 *
 *  Created on: 2015年2月9日
 *      Author: Administrator
 */

#ifndef ZERTCORE_RPCROUTERMANAGER_H_
#define ZERTCORE_RPCROUTERMANAGER_H_

#include <object/PoolObject.h>
#include <utils/tags/TagsValue.h>
#include <concurrent/ConcurrentState.h>

#include "config.h"
#include "RPCConnection.h"

namespace zertcore { namespace concurrent { namespace rpc {
using namespace zertcore::object;
using namespace zertcore::utils;

/**
 * ConnectionConfig
 */
struct ConnectionConfig
{
	RemoteConfig				rc;
	RPCClientConnection::ptr	conn;
};

struct ClientConnectionConfig: ConnectionConfig
{
	condition_group				cond;
};

typedef unordered_multimap<key_type, ClientConnectionConfig>
											key_client_connection_map_type;
typedef unordered_multimap<key_type, ConnectionConfig>
											key_server_connection_map_type;
typedef unordered_map<string, RPCServerConfig>
											rpc_server_map_type;
typedef unordered_map<string, RPCClientConfig>
											rpc_client_map_type;

/**
 * RPCRouterClient
 */
class RPCRouterClient
{
public:
	enum {
		FLAG_NONE							= 0,
		FLAG_SERVER							= 1 << 0,
		FLAG_CLIENT							= 1 << 1,
	};

public:
	typedef ThreadHandler<void ()>			on_handler_type;

public:
	RPCRouterClient() : update_config_flag_(FLAG_NONE) {}

public:
	bool init(const RPCRouterConfig& config);

public:
	/**
	 * Sync to route server
	 */
	void registerRPCHandlers(const key_type& key);
	void registerDataSyncHandlers(const key_type& key, const condition_group& cond);

	/**
	 * Sync data with router server based on update_config_flag_
	 */
	void notifyRouter(ConcurrentState::ptr state);
	void notifyClose(ConcurrentState::ptr state);

public:
	/**
	 * conn must not be null
	 * id would be seed
	 */
	Group<RPCClientConnection::ptr> getServerConnection(const key_type& key);
	Group<RPCClientConnection::ptr> getClientConnections(const key_type& key, const iarchiver_type& iar);

private:
	/**
	 * Got data of a RPCServerConfig
	 * remove all existed data from the server config
	 * add all the keys to key_server_map_
	 */
	void syncServerConfig(key_type, oarchiver_type params);
	/**
	 * Got data of a RPCClientConfig
	 * remove all existed data from the client config
	 * add all the keys to key_client_map_
	 */
	void syncClientConfig(key_type, oarchiver_type params);

public:
	RPCServerConfig& serverConfig() {return server_config_;}
	RPCClientConfig& clientConfig() {return client_config_;}

public:
	void setRPCReadyHandler(const on_handler_type& handler) {
		on_rpc_ready_handler_ = handler;
	}

private:
	RPCClientConnection::ptr	router_conn_;
	on_handler_type				on_rpc_ready_handler_;

private:
	/**
	 * Local config
	 */
	u32							update_config_flag_;
	RPCServerConfig				server_config_;
	RPCClientConfig				client_config_;

	spinlock_type				client_lock_,
								server_lock_;

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
