/*
 * RPCServer.h
 *
 *  Created on: 2015��1��22��
 *      Author: Administrator
 */

#ifndef ZERTCORE_RPCSERVER_H_
#define ZERTCORE_RPCSERVER_H_

#include <pch.h>
#include <net/server/ServerBase.h>

#include "config.h"

namespace zertcore { namespace concurrent { namespace rpc {
using namespace zertcore::net::server;
}}}

/**
 *
 * RPCClient[N1] -- RPCRouter - RPCServer[N2]
 *
 *
 *
 *
 */

namespace zertcore { namespace concurrent { namespace rpc {

/**
 * RPCServer
 */
class RPCServer :
		public ServerBase<RPCServer, RPCConnection>
{
public:

public:
	void init();

public:
	bool registerHandler(const key_type& key, const rpc_handler_type& handler);
};

/**
 * RPC.registerHandler("echo", [](const key_type&, const oachiver_type& params, iachiver_type& ret_data) -> Error {
 * 		string text;
 * 		params["text"] & text;
 * 		ret_data["text"] & text;
 *
 * 		return;
 * });
 */

}}}


#endif /* RPCSERVER_H_ */
