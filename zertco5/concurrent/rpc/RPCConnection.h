/*
 * RPCConnection.h
 *
 *  Created on: 2015年1月31日
 *      Author: Administrator
 */

#ifndef ZERTCORE_RPCCONNECTION_H_
#define ZERTCORE_RPCCONNECTION_H_

#include <net/tcp/PersistConnection.h>
#include "config.h"

namespace zertcore { namespace concurrent { namespace rpc {
using namespace zertcore::net::tcp;

class RPCServer;
class RPCClient;
class RPCRouter;
}}}

namespace zertcore { namespace concurrent { namespace rpc {

/**
 * RPCServerConnection
 */
class RPCServerConnection :
		public PersistConnection<RPCServerConnection, RPCServer>
{
public:

public:
	RPCServerConnection(RPCServer& server);
	virtual ~RPCServerConnection();

public:
	virtual void onPackage(const SharedBuffer& buffer);

public:
	bool response(const SharedBuffer& buffer) {
		return sendPackage(buffer);
	}

/**
private:
	void handleRequest(const SharedBuffer& buffer);
*/

private:
};


/**
 * RPCClientConnection
 */
class RPCClientConnection :
		public PersistConnection<RPCClientConnection, RPCClient>
{

public:
	RPCClientConnection(RPCClient& service);
	virtual ~RPCClientConnection() {
		::printf("~RPCClientConnection()\n");
	}

public:
	bool sendRequest(const SharedBuffer& buffer) {
		return sendPackage(buffer);
	}
	bool sendRequest(const SharedBuffer& buffer, SharedBuffer& receive_buffer) {
		return sendPackage(buffer, receive_buffer);
	}
};

} /* namespace rpc */ } /* namespace concurrent */ } /* namespace zertcore */

#endif /* RPCCONNECTION_H_ */
