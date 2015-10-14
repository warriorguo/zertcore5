/*
 * ServerDetails.hpp
 *
 *  Created on: 2015��1��7��
 *      Author: Administrator
 */

#ifndef ZERTCORE_SERVERBASEDETAILS_HPP_
#define ZERTCORE_SERVERBASEDETAILS_HPP_

#include <pch.h>

#include <thread/ThreadPool.h>
#include <log/Log.h>

namespace zertcore { namespace net { namespace tcp {
using namespace zertcore::concurrent;
}}}

namespace zertcore { namespace net { namespace tcp { namespace server {

template <class Final, class Connection>
bool ServerBase<Final, Connection>::
setup(const ServerConfig& config) {
	if (!IOService<Final>::setup(config))
		return false;

	config_ = config;

	system::error_code err;

	asio::ip::tcp::resolver resolver(this->template io_service_);
	asio::ip::tcp::resolver::query query(config.host, lexical_cast<string>(config.port));
	asio::ip::tcp::endpoint endpoint = *resolver.resolve(query, err);
	if (err) {
		ZCLOG(ERROR) << err.message() << End;
		return false;
	}

	acceptor_.open(endpoint.protocol(), err);
	if (err) {
		ZCLOG(ERROR) << err.message() << End;
		return false;
	}

	acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(true), err);
	if (err) {
		ZCLOG(ERROR) << err.message() << End;
		return false;
	}

	acceptor_.bind(endpoint, err);
	if (err) {
		ZCLOG(ERROR) << err.message() << End;
		return false;
	}

	acceptor_.listen(config.back_log, err);
	if (err) {
		ZCLOG(ERROR) << err.message() << End;
		return false;
	}

	return true;
}

template <class Final, class Connection>
void ServerBase<Final, Connection>::
startAccept() {
	for (u32 i = 0; i < config_.accept_nums; ++i) {
		continueAccept();
	}
}

template <class Final, class Connection>
void ServerBase<Final, Connection>::
onStart() {
	startAccept();
}

template <class Final, class Connection>
void ServerBase<Final, Connection>::
continueAccept() {
	connection_ptr conn = connection_type::template create(*static_cast<final_type *>(this));

	acceptor_.async_accept(conn->template socket(),
		bind(&self::handleAccept, this, asio::placeholders::error, conn));
}

template <class Final, class Connection>
void ServerBase<Final, Connection>::
handleAccept(const system::error_code& err, connection_ptr conn) {
	if (!err) {
		conn->template start();
	}
	else {
		ZCLOG(ERROR) << err.message() << End;
	}

	continueAccept();
}

}}}}


#endif /* SERVERDETAILS_HPP_ */
