/*
 * HttpConnection.cpp
 *
 *  Created on: 2015��1��11��
 *      Author: Administrator
 */

#include "HttpConnection.h"
#include "HttpServer.h"

namespace zertcore { namespace net { namespace tcp {

HttpServerConnection::HttpServerConnection(server::HttpServer& server):
	ConnectionBase<HttpServerConnection, server::HttpServer>(server) {
	;
}

HttpServerConnection::~HttpServerConnection() {
	// TODO Auto-generated destructor stub
	::printf("~HttpServerConnection()\n");
}

size_t HttpServerConnection::
onRead(const SharedBuffer& buffer) {
	u32 handle_size = 0;
	if (!request_.parse(buffer, handle_size)) {
		response_.setStatus(details::HttpResponse::STATUS_BAD_REQUEST);
		return 0;
	}

	::printf("handle_size = %u\n", handle_size);

	if (request_.isDone()) {
		getService().runHandler(thisPtr());
	}

	return handle_size;
}

void HttpServerConnection::
flush() {
	string output = move(response_.output());
	write((const u8 *)output.data(), output.size(), true);
}

}}}
