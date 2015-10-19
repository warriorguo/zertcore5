/*
 * HttpServer.h
 *
 *  Created on: 2015��1��11��
 *      Author: Administrator
 */

#ifndef ZERTCORE_HTTPSERVER_H_
#define ZERTCORE_HTTPSERVER_H_

#include <pch.h>
#include <concurrent/Concurrent.h>

#include <net/tcp/server/ServerBase.h>

#include "HttpConnection.h"

namespace zertcore { namespace net { namespace tcp { namespace server {

/**
 * HttpServer
 */
class HttpServer :
		public ServerBase<HttpServer, HttpServerConnection>
{
public:
	typedef concurrent::ThreadHandler<void (HttpServerConnection::ptr)>
											handler_type;
public:
	virtual ~HttpServer();

public:
	void setHandler(const handler_type& handler) {
		handler_ = handler;
	}

public:
	void runHandler(HttpServerConnection::ptr conn);

private:
	handler_type				handler_;
};

}}}}

#endif /* HTTPSERVER_H_ */
