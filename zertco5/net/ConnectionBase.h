/*
 * Connection.h
 *
 *  Created on: 2015��1��8��
 *      Author: Administrator
 */

#ifndef ZERTCORE_CONNECTION_BASE_H_
#define ZERTCORE_CONNECTION_BASE_H_

#include <pch.h>
#include <utils/types.h>
#include <utils/SharedBuffer.h>
#include <object/PoolObject.h>

namespace zertcore { namespace net {
using namespace zertcore::utils;
using namespace zertcore::object;
}}

#ifndef ZC_CONNECTION_BUFFER_SIZE
#  define ZC_CONNECTION_BUFFER_SIZE			8192
#endif

namespace zertcore { namespace net {

/**
 * ConnectionBase<Final>
 */
template <class Final, class Service, u32 BufferSize = ZC_CONNECTION_BUFFER_SIZE,
	class Socket = asio::ip::tcp::socket>
class ConnectionBase :
		public PoolObject<Final>
{
	typedef ConnectionBase<Final, Service, Socket>
											self;
public:
	const static u32 BUFFER_SIZE			= BufferSize;

public:
	typedef SharedBuffer					buffer_type;
	typedef Service							service_type;
	typedef Socket							socket_type;

public:
	typedef asio::ip::tcp::resolver::iterator
											resolver_type;

public:
	ConnectionBase(service_type& service);
	virtual ~ConnectionBase();

public:
	asio::ip::tcp::socket& socket() {return socket_;}
	void start();
	void read();
	void write(const u8* buffer, size_t size, bool shutdown_next = false);
	void write(const SharedBuffer& buffer, bool shutdown_next = false);
	void shutdown();

	void setError(const string& msg) {
		error_.setError(msg);
	}
	const Error& getError() const {return error_;}

	bool isConnected() const {return is_connected_;}
	bool isLongTerm() const {return long_term_;}
	void setLongTerm(bool flag = true) {
		long_term_ = flag;
	}

public:
	void connect(resolver_type ep);
	resolver_type queryHost(const string& host, u32 port);

public:
	service_type& getService() {return service_;}

public:
	const Error& error() const {return error_;}

protected:
	Error& error() {return error_;}

protected:
	/**
	 * set the error to shutdown the connection
	 * return the bytes that had accepted.
	 */
	virtual size_t onRead(const SharedBuffer& buffer) {return 0;}
	virtual void onConnect();

private:
	void handleRead(const system::error_code& error, size_t bytes_transferred);
	void handleWrite(const system::error_code& error, bool shutdown);
	void handleConnect(const system::error_code& error);

protected:
	service_type&				service_;

protected:
	asio::io_service::strand	strand_;
	socket_type					socket_;

	buffer_type					buffer_;
	size_t						buffer_index_;

	string						send_buffer_;
	bool						send_shutdown_next_;

protected:
	bool						is_connected_;
	bool						long_term_;

protected:
	Error						error_;
};

/**
 * SSLConnectionBase<Final>
 */
/**
template <class Final, class Server>
class SSLConnectionBase :
		public ConnectionBase<Final, Server, asio::ssl::stream<asio::ip::tcp::socket> >
{
public:
	typedef asio::ssl::stream<asio::ip::tcp::socket>
											ssl_socket_type;
	typedef Server							server_type;

public:
	SSLConnectionBase(server_type& server, asio::ssl::context& context);
	virtual ~SSLConnectionBase() {}

public:
	ssl_socket_type::lowest_layer_type& socket() {return socket_.lowest_layer();}
*/
	/**
	 * covered since ssl handshake at first place
	 */
/**
	void start();

protected:
	void handleHandshake(const system::error_code& error);
};
*/

}}

#include "details/ConnectionBaseDetails.hpp"

#endif /* CONNECTION_H_ */
