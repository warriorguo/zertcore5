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
		PersistConnection<RPCServerConnection, RPCServer>(server) {
	// TODO Auto-generated constructor stub

}

RPCServerConnection::~RPCServerConnection() {
	// TODO Auto-generated destructor stub
}

void RPCServerConnection::
onPackage(const SharedBuffer& buffer) {
	oachiver_type o;
	if (!o.buffer(buffer)) {
		ZCLOG(ERROR) >> error() << "Parsing data from [" << getRemoteAddress()
				<< "] failed, shutdown it down." << End;
		return ;
	}

	if (!RPCManager::Instance().pushRemoteCall(o, thisPtr())) {
		ZCLOG(NOTICE) << "pushRemoteCall Failed:" << End;
	}
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
