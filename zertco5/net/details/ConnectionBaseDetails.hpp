/*
 * ConnectionBaseDetails.hpp
 *
 *  Created on: 2015��1��8��
 *      Author: Administrator
 */

#ifndef ZERTCORE_CONNECTIONBASEDETAILS_HPP_
#define ZERTCORE_CONNECTIONBASEDETAILS_HPP_

#include <pch.h>

#include <utils/types.h>

namespace zertcore { namespace net {

template <class Final, class Service, u32 BufferSize, class Socket>
ConnectionBase<Final, Service, BufferSize, Socket>::ConnectionBase(service_type& service) :
	service_(service), strand_(service.getIOService()), socket_(service.getIOService()),
	buffer_(BufferSize), buffer_index_(0), send_shutdown_next_(false),
	is_connected_(false), long_term_(false) {}

template <class Final, class Service, u32 BufferSize, class Socket>
ConnectionBase<Final, Service, BufferSize, Socket>::~ConnectionBase() {
	;
}

template <class Final, class Service, u32 BufferSize, class Socket>
void ConnectionBase<Final, Service, BufferSize, Socket>::
connect(resolver_type ep) {
	asio::async_connect(socket_, ep,
			strand_.wrap(bind(&self::handleConnect, this->template thisPtr(),
					asio::placeholders::error)));
}

template <class Final, class Service, u32 BufferSize, class Socket>
void ConnectionBase<Final, Service, BufferSize, Socket>::
onConnect() {
	ZC_ASSERT(is_connected_);
	if (!send_buffer_.empty()) {
		asio::async_write(socket_,
				asio::buffer(send_buffer_),
				strand_.wrap(bind(&self::handleWrite, this->template thisPtr(),
					asio::placeholders::error, send_shutdown_next_)));
	}
}

template <class Final, class Service, u32 BufferSize, class Socket>
void ConnectionBase<Final, Service, BufferSize, Socket>::
start() {
	is_connected_ = true;
	::printf("start()\n");
	do {
		onConnect();

		if (error_) {
			shutdown();
			return ;
		}
	}
	while(false);

	buffer_index_ = 0;
	read();
}

template <class Final, class Service, u32 BufferSize, class Socket>
void ConnectionBase<Final, Service, BufferSize, Socket>::
read() {
	::printf("read()\n");

	ZC_ASSERT(is_connected_);
	socket_.async_read_some(
			asio::buffer(&buffer_[buffer_index_], buffer_.capacity() - buffer_index_),
			strand_.wrap(bind(&self::handleRead, this->template thisPtr(),
					asio::placeholders::error, asio::placeholders::bytes_transferred)));
}

template <class Final, class Service, u32 BufferSize, class Socket>
void ConnectionBase<Final, Service, BufferSize, Socket>::
write(const u8* buffer, size_t size, bool shutdown_next) {
	if (!is_connected_) {
		if (shutdown_next)
			send_shutdown_next_ = shutdown_next;
		send_buffer_.append((const char *)buffer, size);
		return ;
	}

	asio::write(socket_, asio::buffer(buffer, size));

	if (shutdown_next) {
		shutdown();
	}
}

template <class Final, class Service, u32 BufferSize, class Socket>
void ConnectionBase<Final, Service, BufferSize, Socket>::
write(const SharedBuffer& buffer, bool shutdown_next) {
	write(buffer.data(), buffer.size(), shutdown_next);
}

template <class Final, class Service, u32 BufferSize, class Socket>
void ConnectionBase<Final, Service, BufferSize, Socket>::
asyncWrite(const u8* buffer, size_t size, bool shutdown_next) {
	asio::async_write(socket_, asio::buffer(buffer, size),
		strand_.wrap(bind(&self::handleWrite, this->template thisPtr(),
			asio::placeholders::error, shutdown_next)));
}

template <class Final, class Service, u32 BufferSize, class Socket>
void ConnectionBase<Final, Service, BufferSize, Socket>::
asyncWrite(const SharedBuffer& buffer, bool shutdown_next = false) {
	asyncWrite(buffer.data(), buffer.size(), shutdown_next);
}

template <class Final, class Service, u32 BufferSize, class Socket>
void ConnectionBase<Final, Service, BufferSize, Socket>::
shutdown() {
	::printf("shutdown()\n");
	is_connected_ = false;

	system::error_code ignored_ec;
	socket_.shutdown(asio::ip::tcp::socket::shutdown_both, ignored_ec);
}

template <class Final, class Service, u32 BufferSize, class Socket>
void ConnectionBase<Final, Service, BufferSize, Socket>::
handleWrite(const system::error_code& error, bool shutdown_next) {
	if (error) {
		shutdown();
	}
	if (shutdown_next) {
		shutdown();
	}
}

template <class Final, class Service, u32 BufferSize, class Socket>
void ConnectionBase<Final, Service, BufferSize, Socket>::
handleConnect(const system::error_code& error) {
	start();
}

template <class Final, class Service, u32 BufferSize, class Socket>
void ConnectionBase<Final, Service, BufferSize, Socket>::
handleRead(const system::error_code& error, size_t bytes_transferred) {
	if (error) {
		shutdown();
		return ;
	}

	buffer_index_ += bytes_transferred;
	ZC_ASSERT(buffer_index_ <= buffer_.capacity());

	do {
		/**
		 * got the bytes that had accepted.
		 * would erase them from receive buffer.
		 */
		size_t read_bytes = onRead(buffer_.slice(0, buffer_index_));
		if (error_) {
			shutdown();
			return ;
		}

		if (!read_bytes)
			break;

		if (read_bytes > buffer_index_) {
			buffer_index_ = 0;
			break;
		}
		else {
			buffer_index_ -= read_bytes;
			if (buffer_index_)
				memcpy(&buffer_[0], &buffer_[read_bytes], buffer_index_);
			else
				break;
		}
	}
	while(true);

	/**
	 * buffer_index_ > 0 means theres still data in the buffer
	 */
	if (buffer_index_ > 0 || long_term_) {
		read();
	}
}

template <class Final, class Service, u32 BufferSize, class Socket>
string ConnectionBase<Final, Service, BufferSize, Socket>::
getRemoteAddress() {
	if (!ip_.empty()) {
		return ip_;
	}

	try {
		return ip_ = socket_.remote_endpoint().address().to_string();
	}
	catch(std::exception&) {
		;
	}

	return null;
}

template <class Final, class Service, u32 BufferSize, class Socket>
u32 ConnectionBase<Final, Service, BufferSize, Socket>::
getRemotePort() {
	try {
		return socket_.remote_endpoint().port();
	}
	catch(std::exception&) {
		;
	}

	return 0;
}

}}


#endif /* CONNECTIONBASEDETAILS_HPP_ */
