/*
 * RPCConnection.h
 *
 *  Created on: 2015年1月31日
 *      Author: Administrator
 */

#ifndef ZERTCORE_RPCCONNECTION_H_
#define ZERTCORE_RPCCONNECTION_H_

#include <net/ConnectionBase.h>
#include "config.h"

namespace zertcore { namespace concurrent { namespace rpc {
using namespace zertcore::net;

class RPCServer;
class RPCClient;
}}}

namespace zertcore { namespace concurrent { namespace rpc {

/**
 * RPCServerConnection
 */
class RPCServerConnection :
		public ConnectionBase<RPCServerConnection, RPCServer>
{
public:

public:
	RPCServerConnection(RPCServer& server);
	virtual ~RPCServerConnection();

public:
	virtual size_t onRead(const SharedBuffer& buffer);

private:
	void handleRequest(const SharedBuffer& buffer);

private:
};


/**
 * RPCClientConnection
 */
class RPCClientConnection :
		public ConnectionBase<RPCClientConnection, RPCClient>
{
public:
	RPCClientConnection();
	virtual ~RPCClientConnection();

public:
	virtual size_t onRead(const SharedBuffer& buffer);
};

} /* namespace rpc */ } /* namespace concurrent */ } /* namespace zertcore */

#endif /* RPCCONNECTION_H_ */
