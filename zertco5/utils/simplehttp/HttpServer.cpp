/*
 * HttpServer.cpp
 *
 *  Created on: 2015��1��11��
 *      Author: Administrator
 */

#include "HttpServer.h"
#include "ServerConnection.h"

#include <details.h>

namespace zertcore { namespace net {

HttpServer::~HttpServer() {
	// TODO Auto-generated destructor stub
}

void HttpServer::
handle(SMART_PTR(ServerConnection) conn) {
	/**
	ConcurrentState::ptr state = ConcurrentState::create(callback_type([conn] (RuntimeContext::ptr rc) {
		::printf("on State\n");
		if (!rc.error)
			conn->flush();
	}));
	*/

	handler_.setParams(conn);
	handler_.push();
}

}}
