/*
 * RPCServer.h
 *
 *  Created on: 2015��1��22��
 *      Author: Administrator
 */

#ifndef ZERTCORE_RPCSERVER_H_
#define ZERTCORE_RPCSERVER_H_

#include <pch.h>
#include <net/tcp/server/ServerBase.h>

#include "config.h"
#include "RPCConnection.h"

namespace zertcore { namespace concurrent { namespace rpc {
using namespace zertcore::net::tcp::server;
}}}

namespace zertcore { namespace concurrent { namespace rpc {

/**
 * RPCServer
 */
class RPCServer :
		public ServerBase<RPCServer, RPCServerConnection>
{
public:
	enum {
		TYPE_NONE							= 0,
		TYPE_SERVER							= 1,
		TYPE_DATASYNC						= 2,
	};

public:
	RPCServer() : ServerBase<RPCServer, RPCServerConnection>(), type_(TYPE_NONE) {}

public:
	void setType(const u32& type) {
		type_ = type;
	}

	bool isServer() const {return type_ == TYPE_SERVER;}
	bool isDataSync() const {return type_ == TYPE_DATASYNC;}

private:
	u32							type_;
};

/**
 * RPC.registerHandler("echo", [](const key_type&, const oachiver_type& params, iachiver_type& ret_data) -> void {
 * 		string text;
 * 		params["text"] & text;
 * 		ret_data["text"] & text;
 *
 * 		return;
 * });
 */

}}}


#endif /* RPCSERVER_H_ */
