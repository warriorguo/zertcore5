/*
 * HttpClientConnection.cpp
 *
 *  Created on: 2015年2月1日
 *      Author: Administrator
 */

#include "HttpConnection.h"
#include "HttpClient.h"

namespace zertcore { namespace net { namespace tcp {

HttpClientConnection::HttpClientConnection(client::HttpClient& client) :
	ConnectionBase<HttpClientConnection, client::HttpClient>(client) {
	;
}

HttpClientConnection::~HttpClientConnection() {
	::printf("~HttpClientConnection()\n");
}

void HttpClientConnection::
connect(const string& host, const u32 port) {
	;
}

void HttpClientConnection::
onConnect() {
	;// write HTTP request header
}

size_t HttpClientConnection::
onRead(const SharedBuffer& buffer) {
	return 0;
}

}}}
