/*
 * ServerDetails.hpp
 *
 *  Created on: 2015Äê1ÔÂ7ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_SERVERBASEDETAILS_HPP_
#define ZERTCORE_SERVERBASEDETAILS_HPP_

#include <pch.h>

#include <thread/ThreadPool.h>

namespace zertcore { namespace net {
using namespace zertcore::concurrent;
}}

namespace zertcore { namespace net { namespace server {

template <class Final, class Connection>
bool ServerBase<Final, Connection>::
init(const ServerConfig& config) {
	if (!IOService<Final>::init(config))
		return false;

	asio::ip::tcp::resolver resolver(this->template io_service_);
	asio::ip::tcp::resolver::query query(config.host, lexical_cast<string>(config.port));
	asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);

	acceptor_.open(endpoint.protocol());
	acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(true));
	acceptor_.bind(endpoint);
	acceptor_.listen();

	startAccept(config.accept_nums);
	return true;
}

template <class Final, class Connection>
void ServerBase<Final, Connection>::
startAccept(const u32& amount) {
	for (u32 i = 0; i < amount; ++i) {
		connection_ptr conn = connection_type::template create(*static_cast<final_type *>(this));

		acceptor_.async_accept(conn->template socket(),
			bind(&self::handleAccept, this, asio::placeholders::error, conn));
	}
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

	continueAccept();
}

}}}


#endif /* SERVERDETAILS_HPP_ */
