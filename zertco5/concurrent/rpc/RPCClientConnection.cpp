/*
 * RPCClientConnection.cpp
 *
 *  Created on: 2015年2月4日
 *      Author: Administrator
 */

#include "RPCConnection.h"
#include "RPCClient.h"
#include "RPCSpec.h"

#include <details.h>

namespace zertcore { namespace concurrent { namespace rpc {

RPCClientConnection::RPCClientConnection(RPCClient& service) :
	PersistConnection<RPCClientConnection, RPCClient>(service) {
	;
}

}}}
