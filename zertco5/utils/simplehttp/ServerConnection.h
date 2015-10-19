/*
 * ServerConnection.h
 *
 *  Created on: 2015年10月18日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_SIMPLEHTTP_SERVERCONNECTION_H_
#define ZERTCORE_UTILS_SIMPLEHTTP_SERVERCONNECTION_H_

#include <pch.h>
#include <net/tcp/ConnectionBase.h>

#include "HttpParser.h"
#include "HttpResponse.h"

namespace zertcore { namespace net {
using namespace zertcore::utils;

class HttpServer;
}}

namespace zertcore { namespace net {

/**
 * ServerConnection
 */
class ServerConnection :
		public tcp::ConnectionBase<ServerConnection, HttpServer>
{
public:
	ServerConnection(HttpServer& server);
	virtual ~ServerConnection() {}

public:
	const HttpContext& context() const { return context_; }
	const string& get(const string& key);
	const string& cookie(const string& key);

public:
	bool response(const http::status_type& status, const SharedBuffer& content);
	bool response(const http::status_type& status);
	bool response(const SharedBuffer& content);

public:
	virtual size_t onRead(const SharedBuffer& buffer);

private:

private:
	HttpContext					context_;
	HttpResponse				response_;
};

}}

#endif /* UTILS_SIMPLEHTTP_SERVERCONNECTION_H_ */
