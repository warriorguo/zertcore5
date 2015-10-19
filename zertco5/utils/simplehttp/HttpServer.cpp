/*
 * HttpServer.cpp
 *
 *  Created on: 2015��1��11��
 *      Author: Administrator
 */

#include "HttpServer.h"
#include "ServerConnection.h"

#include <details.h>

namespace zertcore { namespace net {

HttpServer::HttpServer() : inited_(false) {
	config_.host = "0.0.0.0";
	config_.port = 80;
	config_.thread_nums = 6;
}

HttpServer::~HttpServer() {}

bool HttpServer::
init() {
	if (inited_) {
		return false;
	}

	return inited_ = setup(config_);
}

bool HttpServer::
setConfig(const ServerConfig& config) {
	if (inited_)
		return false;

	config_ = config;
	return true;
}

void HttpServer::
handle(SMART_PTR(ServerConnection) conn) {
	/**
	ConcurrentState::ptr state = ConcurrentState::create(callback_type([conn] (RuntimeContext::ptr rc) {
		::printf("on State\n");
		if (!rc.error)
			conn->flush();
	}));
	*/

	handler_.setParams(conn);
	handler_.push();
}

}}
