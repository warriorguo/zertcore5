/*
 * RPCClient.h
 *
 *  Created on: 2015年1月27日
 *      Author: Administrator
 */

#ifndef ZERTCORE_RPCCLIENT_H_
#define ZERTCORE_RPCCLIENT_H_

#include <pch.h>
#include <net/client/ClientBase.h>

#include "config.h"
#include "RPCConnection.h"

namespace zertcore { namespace concurrent { namespace rpc {
using namespace zertcore::net::client;
}}}

namespace zertcore { namespace concurrent { namespace rpc {

/**
 * RPCClient
 */
class RPCClient :
		public ClientBase<RPCClient, RPCClientConnection>
{
public:
	void sendRequest(const SharedBuffer& buf);

private:
};

}}}


#endif /* RPCCLIENT_H_ */
