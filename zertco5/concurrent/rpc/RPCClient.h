/*
 * RPCClient.h
 *
 *  Created on: 2015年1月27日
 *      Author: Administrator
 */

#ifndef ZERTCORE_RPCCLIENT_H_
#define ZERTCORE_RPCCLIENT_H_

#include <pch.h>
#include <net/tcp/client/ClientBase.h>
#include <utils/Singleton.h>

#include "config.h"
#include "RPCConnection.h"

namespace zertcore { namespace concurrent { namespace rpc {
using namespace zertcore::net::tcp::client;
using namespace zertcore::utils;
}}}

namespace zertcore { namespace concurrent { namespace rpc {

/**
 * RPCClient
 */
class RPCClient :
		public ClientBase<RPCClient, RPCClientConnection>,
		public Singleton<RPCClient, NoneChecker>
{
public:
	RPCClient() : ClientBase<RPCClient, RPCClientConnection>() {}
	virtual ~RPCClient() {}
};

}}}


#endif /* RPCCLIENT_H_ */
