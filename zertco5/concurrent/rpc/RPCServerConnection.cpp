/*
 * RPCConnection.cpp
 *
 *  Created on: 2015年1月31日
 *      Author: Administrator
 */

#include "RPCConnection.h"
#include "RPCServer.h"
#include "RPCSpec.h"

#include <details.h>

namespace zertcore { namespace concurrent { namespace rpc {

RPCServerConnection::RPCServerConnection(RPCServer& server) :
		PersistConnection<RPCServerConnection, RPCServer>(server) {
	// TODO Auto-generated constructor stub

}

RPCServerConnection::~RPCServerConnection() {
	::printf("~RPCServerConnection()\n");
}

void RPCServerConnection::
onPackage(const SharedBuffer& buffer) {

}

} /* namespace rpc */ } /* namespace concurrent */ } /* namespace zertcore */
