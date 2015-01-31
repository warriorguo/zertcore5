/*
 * HttpConnection.h
 *
 *  Created on: 2015��1��11��
 *      Author: Administrator
 */

#ifndef ZERTCORE_HTTPCONNECTION_H_
#define ZERTCORE_HTTPCONNECTION_H_

#include <pch.h>

#include <net/ConnectionBase.h>

#include "HttpRequest.h"
#include "HttpResponse.h"

namespace zertcore { namespace net { namespace server {
class HttpServer;
}}}

namespace zertcore {namespace net {

/**
 * HttpConnection
 */
class HttpConnection :
		public ConnectionBase<HttpConnection, server::HttpServer>
{
public:
	HttpConnection(server::HttpServer& server);

public:
	virtual ~HttpConnection();

public:
	void flush();

public:
	virtual size_t onRead(const SharedBuffer& buffer);

public:
	details::HttpRequest& request() {return request_;}
	details::HttpResponse& response() {return response_;}

	ZC_TO_STRING("HttpConnection");

private:
	details::HttpRequest		request_;
	details::HttpResponse		response_;
};

} /* namespace net */} /* namespace zertcore */

#endif /* HTTPCONNECTION_H_ */
