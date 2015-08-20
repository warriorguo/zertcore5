/*
 * HttpServer.cpp
 *
 *  Created on: 2015��1��11��
 *      Author: Administrator
 */

#include "HttpServer.h"

namespace zertcore {namespace net { namespace tcp { namespace server {

HttpServer::~HttpServer() {
	// TODO Auto-generated destructor stub
}

void HttpServer::
runHandler(HttpServerConnection::ptr conn) {
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

}}}}
