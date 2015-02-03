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
	if (buffer.size() < sizeof(u32))
		return 0;

	u32 length = *((const u32 *)buffer.data());
	if (length > RPC_MAX_PACKAGE_SIZE) {
		setError("length was too large");
		return 0;
	}
	if (length > buffer.size())
		return 0;

	handleRequest(buffer.slice(0, length));
	return length;
}

void RPCServerConnection::
handleRequest(const SharedBuffer& buffer) {
	oachiver_type o;
	if (!o.buffer(buffer)) {
		setError("Parse failed");
		return ;
	}

	RPCManager::Instance().putRemoteCall(o, thisPtr());
}

} /* namespace rpc */ } /* namespace concurrent */ } /* namespace zertcore */
