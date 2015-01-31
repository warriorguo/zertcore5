/*
 * HttpConnection.cpp
 *
 *  Created on: 2015��1��11��
 *      Author: Administrator
 */

#include "HttpConnection.h"
#include "HttpServer.h"

namespace zertcore {namespace net {

HttpConnection::HttpConnection(server::HttpServer& server):
	ConnectionBase<HttpConnection, server::HttpServer>(server) {
	;
}

HttpConnection::~HttpConnection() {
	// TODO Auto-generated destructor stub
	::printf("~HttpConnection()\n");
}

size_t HttpConnection::
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

void HttpConnection::
flush() {
	string output = move(response_.output());
	write((const u8 *)output.data(), output.size(), true);
}

} /* namespace net */} /* namespace zertcore */
