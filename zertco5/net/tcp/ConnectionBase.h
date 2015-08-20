/*
 * Connection.h
 *
 *  Created on: 2015��1��8��
 *      Author: Administrator
 */

#ifndef ZERTCORE_TCP_CONNECTION_BASE_H_
#define ZERTCORE_TCP_CONNECTION_BASE_H_

#include <pch.h>
#include <utils/types.h>
#include <utils/buffer/SharedBuffer.h>
#include <object/PoolObject.h>
#include <thread/ThreadHandler.h>

#include "config.h"

namespace zertcore { namespace net { namespace tcp {
using namespace zertcore::utils;
using namespace zertcore::object;
using namespace zertcore::concurrent;
}}}

#ifndef ZC_CONNECTION_BUFFER_SIZE
#  define ZC_CONNECTION_BUFFER_SIZE			8192
#endif

namespace zertcore { namespace net { namespace tcp {

/**
 * ConnectionBase<Final>
 */
template <class Final, class Service, u32 BufferSize = ZC_CONNECTION_BUFFER_SIZE,
	class Socket = asio::ip::tcp::socket>
class ConnectionBase :
		public PoolObject<Final>
{
	typedef ConnectionBase<Final, Service, BufferSize, Socket>
											self;
public:
	typedef ThreadHandler<void (typename self::ptr)>
											shutdown_handler_type;

public:
	enum {
		TYPE_NONE							= 0,
		TYPE_CLIENT							= 1,
		TYPE_SERVER							= 2,
	};

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
	socket_type& socket() {return socket_;}
	void start();
	size_t read(u8* buffer, size_t size);
	size_t read(buffer_type& buffer);
	bool asyncRead();

	/**
	 * block write
	 */
	bool write(const u8* buffer, size_t size, bool shutdown_next = false);
	bool write(const buffer_type& buffer, bool shutdown_next = false);

	/**
	 * nonblock write
	 */
	bool asyncWrite(const u8* buffer, size_t size, bool shutdown_next = false);
	bool asyncWrite(const buffer_type& buffer, bool shutdown_next = false);

	void shutdown();

	void setError(const string& msg) {
		error_.setError(msg);
	}
	const Error& getError() const {return error_;}

	bool isConnected() const {return is_connected_;}

public:
	bool connect(resolver_type ep);
	bool reconnect();

	resolver_type queryHost(const string& host, u32 port);

public:
	service_type& getService() {return service_;}

public:
	const RemoteConfig& getRemoteConfig();

public:
	const Error& error() const {return error_;}

public:
	static void setShutdownHandler(const shutdown_handler_type& handler);

public:
	Error& error() {return error_;}

protected:
	/**
	 * set the error to shutdown the connection
	 * return the bytes that had accepted.
	 */
	virtual size_t onRead(const SharedBuffer& buffer) {return 0;}
	virtual void onConnect();

	virtual void onShutdown() {return;}

private:
	void doAsyncRead();

	void handleRead(const system::error_code& error, buffer_type buffer, size_t bytes_transferred);
	void handleWrite(const system::error_code& error, buffer_type buffer, bool shutdown_next);
	/**
	void handleConnect(const system::error_code& error);
	*/
protected:
	service_type&				service_;

protected:
	asio::io_service::strand	strand_;
	socket_type					socket_;

	buffer_type					buffer_;
	buffer_type					read_buffer_;
	bool						is_in_reading_;

protected:
	resolver_type				resolver_ep_;
	bool						is_connected_;

protected:
	RemoteConfig				rc_;

protected:
	Error						error_;

protected:
	u32							service_type_;

	ZC_TO_STRING(
		"ip" << rc_.toString() <<
		"is_connected" << is_connected_ <<
		"type" << service_type_ <<
		"error" << error_.message
	);

private:
	spinlock_type				read_lock_;
	spinlock_type				write_lock_;

private:
	static shutdown_handler_type
								shutdown_handler_;
};

template <class Final, class Service, u32 BufferSize, class Socket>
typename ConnectionBase<Final, Service, BufferSize, Socket>::shutdown_handler_type
								ConnectionBase<Final, Service, BufferSize, Socket>::shutdown_handler_;

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

}}}

namespace zertcore { namespace net { namespace tcp {

template <typename Conn>
void writeConn(typename Conn::ptr conn, const SharedBuffer& buffer) {
	conn->write(buffer);
}

}}}

#include "details/ConnectionBaseDetails.hpp"

#endif /* CONNECTION_H_ */
