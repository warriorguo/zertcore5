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
	HttpServer();
	virtual ~HttpServer();

	/**
	 * must set config before init
	 */
	bool setConfig(const ServerConfig& config);
	bool init();

public:
	void setHandler(const handler_type& handler) {
		handler_ = handler;
	}

public:
	void handle(SMART_PTR(ServerConnection) conn);

private:
	bool						inited_;
	ServerConfig				config_;
	handler_type				handler_;
};

}}

#endif /* HTTPSERVER_H_ */
