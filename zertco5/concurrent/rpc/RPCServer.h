/*
 * RPCServer.h
 *
 *  Created on: 2015Äê1ÔÂ22ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_RPCSERVER_H_
#define ZERTCORE_RPCSERVER_H_

#include <pch.h>
#include <net/server/ServerBase.h>

#include <thread/ThreadHandler.h>

#include <serialize/Serializer.h>
#include <serialize/Unserializer.h>

namespace zertcore { namespace concurrent { namespace rpc {
using namespace zertcore::net::server;
using namespace zertcore::serialization;
}}}

namespace zertcore { namespace concurrent { namespace rpc {

class RPCConnection;

class RPCServer :
		public ServerBase<RPCServer, RPCConnection>
{
public:
	typedef Serializer<MsgPackIStream>		iachiver_type;
	typedef Unserializer<MsgPackOStream>	oachiver_type;
	typedef ThreadHandler<void (const key_type&, iachiver_type& achiver)>
											rpc_handler_type;

public:
	void init();

public:
	bool registerHandler(const key_type& key, const rpc_handler_type& handler);

	//this should be in RPC Client
	bool registerDataSyncHandler(const key_type& key, const rpc_handler_type& handler);
};

/**
 * RPC.registerHandler("echo", [](const key_type&, oachiver_type& params, iachiver_type& ret_data) {
 * 		string text;
 * 		params["text"] & text;
 * 		ret_data["text"] & text;
 *
 * 		return;
 * });
 */

}}}


#endif /* RPCSERVER_H_ */
