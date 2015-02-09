/*
 * RPCClientConnection.cpp
 *
 *  Created on: 2015年2月4日
 *      Author: Administrator
 */

#include "RPCConnection.h"
#include "RPCClient.h"
#include "RPCManager.h"

namespace zertcore { namespace concurrent { namespace rpc {

void RPCClientConnection::
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

}}}
