/*
 * ServerConnection.cpp
 *
 *  Created on: 2015年10月18日
 *      Author: Administrator
 */

#include "ServerConnection.h"
#include "HttpServer.h"

#include <details.h>

namespace zertcore { namespace net {

ServerConnection::ServerConnection(HttpServer& server):
	ConnectionBase<ServerConnection, HttpServer>(server) {
	;
}

const string& ServerConnection::
get(const string& key) {
	return context_.getQuery(key);
}

const string& ServerConnection::
cookie(const string& key) {
	return context_.getCookie(key);
}

bool ServerConnection::
response(const http::status_type& status, const SharedBuffer& content) {
	if (!response_.response(status, content)) {
		ZCLOG(ERROR) << "Response failed" << End;
		return false;
	}

	write(response_.output(), true);
	return true;
}

bool ServerConnection::
response(const http::status_type& status) {
	return response(status, SharedBuffer());
}

bool ServerConnection::
response(const SharedBuffer& content) {
	return response(http::STATUS_OK, content);
}

size_t ServerConnection::
onRead(const SharedBuffer& buffer) {
	ret_type ret = RET_OK;

	if (context_.header.method)
		ret = parseHttp(buffer, context_);
	else {
		//TODO: ugly code here, try to rewrite the ConnectionBase buffer control
		SharedBuffer sb(buffer);
		sb.getOwned();

		ret = parseHttp(sb, context_);
	}

	switch (ret) {
	case RET_OK:
		getService().handle(thisPtr());

	case RET_CONTINUE:
		return buffer.size();

	case RET_ERROR:
		response(http::STATUS_BAD_REQUEST);
		ZCLOG(ERROR) >> error() << "Parse HTTP protocol failed" << End;
		return 0;
	}

	ZC_DEBUG_ASSERT(0); // never reach here.
	return 0;
}

}}


