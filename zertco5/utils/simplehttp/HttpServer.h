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

namespace zertcore { namespace net {

class ServerConnection;

/**
 * HttpServer
 */
class HttpServer :
		public tcp::server::ServerBase<HttpServer, ServerConnection>
{
public:
	typedef concurrent::ThreadHandler<void (SMART_PTR(ServerConnection))>
											handler_type;
public:
	virtual ~HttpServer();

public:
	void setHandler(const handler_type& handler) {
		handler_ = handler;
	}

public:
	void handle(SMART_PTR(ServerConnection) conn);

private:
	handler_type				handler_;
};

}}

#endif /* HTTPSERVER_H_ */
