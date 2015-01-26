/*
 * ClientBaseDetails.hpp
 *
 *  Created on: 2015Äê1ÔÂ13ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_CLIENTBASEDETAILS_HPP_
#define ZERTCORE_CLIENTBASEDETAILS_HPP_

#include <pch.h>

namespace zertcore { namespace net { namespace client {

template <class Final, class Connection>
bool ClientBase<Final, Connection>::
init(const ClientConfig& config) {
	if (!IOService<Final>::init(config))
		return false;

	return true;
}

template <class Final, class Connection>
typename ClientBase<Final, Connection>::connection_ptr ClientBase<Final, Connection>::
connect(const string& host, const u32& port) {
	connection_ptr conn = connection_type::template create(*static_cast<final_type *>(this));
	if (!conn) return null;

	asio::ip::tcp::resolver::query query(host, lexical_cast<string>(port));
	resolver_.async_resolve(query,
		bind(&self::handleResolve, this, asio::placeholders::error,
			asio::placeholders::iterator, conn));

	return conn;
}

template <class Final, class Connection>
void ClientBase<Final, Connection>::
handleResolve(const system::error_code& err,
		asio::ip::tcp::resolver::iterator ep, connection_ptr conn) {
	conn->template connect(ep);
}

}}}

#endif /* CLIENTBASEDETAILS_HPP_ */
