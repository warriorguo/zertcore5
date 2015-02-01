/*
 * HttpServer.cpp
 *
 *  Created on: 2015��1��11��
 *      Author: Administrator
 */

#include "HttpServer.h"

namespace zertcore {namespace net {namespace server {

HttpServer::~HttpServer() {
	// TODO Auto-generated destructor stub
}

void HttpServer::
runHandler(HttpServerConnection::ptr conn) {
	concurrent::handler_type handler =
			handler_.setParams<concurrent::handler_type::type>(conn);

	/**
	ConcurrentState::ptr state = ConcurrentState::create(callback_type([conn] (const RunningContext& rc) {
		::printf("on State\n");
		if (!rc.error)
			conn->flush();
	}));
	*/

	Concurrent::Instance().add(handler);
}

} /* namespace server */} /* namespace net */} /* namespace zertcore */
