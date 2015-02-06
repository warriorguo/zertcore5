/*
 * RPCConnection.cpp
 *
 *  Created on: 2015年1月31日
 *      Author: Administrator
 */

#include "RPCConnection.h"
#include "RPCServer.h"
#include "RPCManager.h"

namespace zertcore { namespace concurrent { namespace rpc {

RPCServerConnection::RPCServerConnection(RPCServer& server) :
		ConnectionBase<RPCServerConnection, RPCServer>(server) {
	// TODO Auto-generated constructor stub

}

RPCServerConnection::~RPCServerConnection() {
	// TODO Auto-generated destructor stub
}

size_t RPCServerConnection::
onRead(const SharedBuffer& buffer) {
}

void RPCServerConnection::
handleRequest(const SharedBuffer& buffer) {
	oachiver_type o;
	if (!o.buffer(buffer)) {
		setError("Parse failed");
		return ;
	}

	RPCManager::Instance().pushRemoteCall(o, thisPtr());
}

} /* namespace rpc */ } /* namespace concurrent */ } /* namespace zertcore */
