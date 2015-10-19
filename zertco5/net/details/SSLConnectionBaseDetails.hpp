/*
 * SSLConnectionBaseDetails.hpp
 *
 *  Created on: 2015Äê1ÔÂ10ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_SSLCONNECTIONBASEDETAILS_HPP_
#define ZERTCORE_SSLCONNECTIONBASEDETAILS_HPP_

#include <pch.h>

#include <utils/types.h>

namespace zertcore { namespace net {

template <class Final, class Server>
SSLConnectionBase<Final, Server>::
SSLConnectionBase(server_type& server, asio::ssl::context& context) :
	server_(server), strand_(server.IOService()), socket_(server.IOService(), context),
	buffer_index_(0), long_term_(false) {
	buffer_.reserve(ZC_CONNECTION_BUFFER_SIZE);
}

template <class Final, class Server>
void SSLConnectionBase<Final, Server>::
start() {
	socket_.async_handshake(asio::ssl::stream_base::server,
		strand_.wrap(bind(&SSLConnectionBase<Final, Server>::handleHandshake, thisPtr(),
			asio::placeholders::error)));
}

template <class Final, class Server>
void SSLConnectionBase<Final, Server>::
handleHandshake(const system::error_code& error) {
	if (!error) {
		ConnectionBase<Final, ssl_socket_type>::start();
	}
}

}}

#endif /* SSLCONNECTIONBASEDETAILS_HPP_ */
