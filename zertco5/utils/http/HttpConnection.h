/*
 * HttpConnection.h
 *
 *  Created on: 2015��1��11��
 *      Author: Administrator
 */

#ifndef ZERTCORE_HTTPCONNECTION_H_
#define ZERTCORE_HTTPCONNECTION_H_

#include <pch.h>

#include <net/tcp/ConnectionBase.h>

#include "HttpRequest.h"
#include "HttpResponse.h"

namespace zertcore { namespace net { namespace tcp { namespace server {
class HttpServer;
}}}}

namespace zertcore { namespace net { namespace tcp { namespace client {
class HttpClient;
}}}}

namespace zertcore { namespace net { namespace tcp {

/**
 * HttpServerConnection
 */
class HttpServerConnection :
		public ConnectionBase<HttpServerConnection, server::HttpServer>
{
public:
	HttpServerConnection(server::HttpServer& server);

public:
	virtual ~HttpServerConnection();

public:
	void flush();

public:
	virtual size_t onRead(const SharedBuffer& buffer);

public:
	details::HttpRequest& request() {return request_;}
	details::HttpResponse& response() {return response_;}

private:
	details::HttpRequest		request_;
	details::HttpResponse		response_;

	ZC_TO_STRING("HttpServerConnection");
};

}}}

namespace zertcore { namespace net { namespace tcp {

/**
 * HttpClientConnection
 */
class HttpClientConnection :
		public ConnectionBase<HttpClientConnection, client::HttpClient>
{
public:
	HttpClientConnection(client::HttpClient& client);

public:
	virtual ~HttpClientConnection();

public:
	void connect(const string& host, const u32 port);

	virtual void onConnect();
	virtual size_t onRead(const SharedBuffer& buffer);

private:
	ZC_TO_STRING("HttpClientConnection");
};

}}}

#endif /* HTTPCONNECTION_H_ */
