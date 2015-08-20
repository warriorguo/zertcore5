/*
 * RPCRouter.h
 *
 *  Created on: 2015年1月27日
 *      Author: Administrator
 */

#ifndef ZERTCORE_RPCROUTER_H_
#define ZERTCORE_RPCROUTER_H_

#include <pch.h>
#include <net/tcp/server/ServerBase.h>

#include "config.h"
#include "RPCServer.h"
#include "RPCClient.h"

namespace zertcore { namespace concurrent { namespace rpc {

/**
 * RPCRouter
 * Mid layer for RPC framework
 * Distribute the server & client info
 * check the healthy of each client
 */
class RPCRouter
{
public:
	typedef unordered_map<string, RPCClientConnection::ptr>
											conn_map_type;

	enum {
		TIMEOUT								= 100,
	};

public:
	RPCRouter();

public:
	bool globalInit();

private:
	void SyncServer(key_type, oarchiver_type params, iarchiver_type& ret_data);
	void SyncClient(key_type, oarchiver_type params, iarchiver_type& ret_data);

private:
	void SyncData();

private:
	void onConnectionClose(RPCClientConnection::ptr conn);

private:
	RPCServer					server_;

private:
	unordered_map<string, RPCServerConfig>
								svr_info_map_;
	unordered_map<string, RPCClientConfig>
								cli_info_map_;

private:
	conn_map_type				conn_map_;

private:
	uuid_t						id_counter_;
};

}}}

#endif /* RPCROUTER_H_ */
