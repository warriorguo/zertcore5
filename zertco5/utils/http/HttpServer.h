/*
 * HttpServer.h
 *
 *  Created on: 2015Äê1ÔÂ11ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_HTTPSERVER_H_
#define ZERTCORE_HTTPSERVER_H_

#include <pch.h>
#include <concurrent/Concurrent.h>

#include <net/server/ServerBase.h>

#include "HttpConnection.h"

namespace zertcore { namespace net { namespace server {

/**
 * HttpServer
 */
class HttpServer :
		public ServerBase<HttpServer, HttpConnection>
{
public:
	typedef concurrent::ThreadHandler<void (HttpConnection::ptr)>
											handler_type;

public:
	virtual ~HttpServer();

public:
	void setHandler(const handler_type& handler) {
		handler_ = handler;
	}

public:
	void runHandler(HttpConnection::ptr conn);

private:
	handler_type				handler_;
};

} /* namespace server */} /* namespace net */} /* namespace zertcore */

#endif /* HTTPSERVER_H_ */
