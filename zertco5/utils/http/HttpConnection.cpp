/*
 * HttpConnection.cpp
 *
 *  Created on: 2015Äê1ÔÂ11ÈÕ
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
onRead(const u8* buffer, size_t size) {
	u32 handle_size = 0;
	if (!request_.parse(string((const string::value_type *)buffer, size), handle_size)) {
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
