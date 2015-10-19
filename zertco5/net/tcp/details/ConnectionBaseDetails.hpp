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
#include <log/Log.h>

namespace zertcore { namespace net { namespace tcp {

template <class Final, class Service, u32 BufferSize, class Socket>
ConnectionBase<Final, Service, BufferSize, Socket>::ConnectionBase(service_type& service) :
	service_(service), strand_(service.getIOService()), socket_(service.getIOService()),
	is_in_reading_(false), is_connected_(false), service_type_(0) {
	work_tid_ = Thread::getCurrentTid();
}

template <class Final, class Service, u32 BufferSize, class Socket>
ConnectionBase<Final, Service, BufferSize, Socket>::~ConnectionBase() {
	;
}

template <class Final, class Service, u32 BufferSize, class Socket>
bool ConnectionBase<Final, Service, BufferSize, Socket>::
connect(resolver_type ep) {
	::printf("connecting\n");

	service_type_ = TYPE_CLIENT;
	resolver_ep_ = ep;

	system::error_code ec;
	asio::connect(socket_, ep, ec);

	if (!ec) {
		onConnect();
		return true;
	}

	return false;
}

template <class Final, class Service, u32 BufferSize, class Socket>
bool ConnectionBase<Final, Service, BufferSize, Socket>::
reconnect() {
	if (service_type_ != TYPE_CLIENT)
		return false;

	return connect(resolver_ep_);
}

template <class Final, class Service, u32 BufferSize, class Socket>
void ConnectionBase<Final, Service, BufferSize, Socket>::
onConnect() {
	::printf("connect()\n");
	is_connected_ = true;
	socket_.set_option(asio::ip::tcp::no_delay(true));
}

template <class Final, class Service, u32 BufferSize, class Socket>
void ConnectionBase<Final, Service, BufferSize, Socket>::
start() {
	::printf("start():%s[%u]\n", getRemoteConfig().host.c_str(), getRemoteConfig().port);

	if (!service_type_) {
		service_type_ = TYPE_SERVER;
	}

	do {
		onConnect();

		if (error_) {
			shutdown();
			return ;
		}
	}
	while(false);

	ZC_ASSERT( asyncRead() );
}

template <class Final, class Service, u32 BufferSize, class Socket>
bool ConnectionBase<Final, Service, BufferSize, Socket>::
write(const u8* buffer, size_t size, bool shutdown_next) {
	SharedBuffer sbuffer;
	sbuffer.assign(buffer, size);

	return write(sbuffer, size);
}

template <class Final, class Service, u32 BufferSize, class Socket>
bool ConnectionBase<Final, Service, BufferSize, Socket>::
write(const SharedBuffer& buffer, bool shutdown_next) {
	if (Thread::getCurrentTid() == work_tid_) {
		return doWrite(buffer, shutdown_next);
	}

	ThreadHandler<bool ()> handler(bind(&self::doWrite, this, buffer, shutdown_next));
	handler.setThreadIndex(work_tid_);
	return handler.push();
}

template <class Final, class Service, u32 BufferSize, class Socket>
bool ConnectionBase<Final, Service, BufferSize, Socket>::
doWrite(const buffer_type& buffer, bool shutdown_next) {
	ZC_ASSERT(Thread::getCurrentTid() == work_tid_);

	if (!is_connected_ && !reconnect())
		return false;

	system::error_code ec;
	asio::write(socket_, asio::buffer(buffer.data(), buffer.size()), ec);

	if (ec) {
		ZCLOG(ERROR) << "write failed:" << ec.message() << End;

		shutdown();
		return false;
	}

	if (shutdown_next) {
		shutdown();
		return true;
	}

	return true;
}

template <class Final, class Service, u32 BufferSize, class Socket>
bool ConnectionBase<Final, Service, BufferSize, Socket>::
asyncWrite(const u8* buffer, size_t size, bool shutdown_next) {
	SharedBuffer sbuffer;
	sbuffer.assign(buffer, size);

	return asyncWrite(sbuffer, shutdown_next);
}

template <class Final, class Service, u32 BufferSize, class Socket>
bool ConnectionBase<Final, Service, BufferSize, Socket>::
asyncWrite(const SharedBuffer& buffer, bool shutdown_next) {
	if (!is_connected_ && !reconnect())
		return false;

	asio::async_write(socket_, asio::buffer(buffer.data(), buffer.size()),
		strand_.wrap(bind(&self::handleWrite, this->template thisPtr(),
				asio::placeholders::error, buffer, shutdown_next) ));

	return true;
}

template <class Final, class Service, u32 BufferSize, class Socket>
void ConnectionBase<Final, Service, BufferSize, Socket>::
handleWrite(const system::error_code& error, buffer_type buffer, bool shutdown_next) {
	if (error) {
		ZCLOG(ERROR) << "handleWrite(): Connection write failed" << End;

		shutdown();
		return ;
	}

	if (shutdown_next) {
		shutdown();
	}
	else {
		asyncRead();
	}
/**
	if (!write_buffer_.empty()) {
		doAsyncWrite();
	}
*/
}

template <class Final, class Service, u32 BufferSize, class Socket>
void ConnectionBase<Final, Service, BufferSize, Socket>::
setShutdownHandler(const shutdown_handler_type& handler) {
	shutdown_handler_ = handler;
}

template <class Final, class Service, u32 BufferSize, class Socket>
void ConnectionBase<Final, Service, BufferSize, Socket>::
shutdown() {
	::printf("shutdown():%u IP:%s[%u]\n", service_type_, getRemoteConfig().host.c_str(), getRemoteConfig().port);
	is_connected_ = false;

	if (buffer_.writable())
		buffer_.setSize(0);

	onShutdown();
	if (shutdown_handler_) {
		shutdown_handler_.setParams(this->template thisPtr());
		shutdown_handler_.push();
	}

	system::error_code ignored_ec;
	socket_.shutdown(asio::ip::tcp::socket::shutdown_both, ignored_ec);
}

/**
template <class Final, class Service, u32 BufferSize, class Socket>
void ConnectionBase<Final, Service, BufferSize, Socket>::
handleConnect(const system::error_code& error) {
	start();
}
*/

template <class Final, class Service, u32 BufferSize, class Socket>
size_t ConnectionBase<Final, Service, BufferSize, Socket>::
read(u8* buffer, size_t size) {
	system::error_code ec;
	size_t receive = asio::read(socket_, asio::buffer(buffer, size), ec);

	if (ec) {
		ZCLOG(ERROR) << "read failed:" << ec.message() << End;
		return 0;
	}

	return receive;
}

template <class Final, class Service, u32 BufferSize, class Socket>
size_t ConnectionBase<Final, Service, BufferSize, Socket>::
read(SharedBuffer& buffer) {
	return read(buffer.data(), buffer.capacity());
}

template <class Final, class Service, u32 BufferSize, class Socket>
void ConnectionBase<Final, Service, BufferSize, Socket>::
doAsyncRead() {
	if (!is_connected_) return ;
	read_buffer_.setSize(0);

	socket_.async_read_some(
			asio::buffer(&read_buffer_[0], read_buffer_.capacity()),
			strand_.wrap(bind(&self::handleRead, this->template thisPtr(),
					asio::placeholders::error, read_buffer_, asio::placeholders::bytes_transferred)));
}

template <class Final, class Service, u32 BufferSize, class Socket>
bool ConnectionBase<Final, Service, BufferSize, Socket>::
asyncRead() {
	spinlock_guard_type guard(read_lock_);

	if (!is_connected_)
		return false;

	if (is_in_reading_) {
		return true;
	}
	is_in_reading_ = true;

	if (!read_buffer_.capacity()) {
		ZC_ASSERT(read_buffer_.reserve(BufferSize));
	}

	doAsyncRead();
	return true;
}

template <class Final, class Service, u32 BufferSize, class Socket>
void ConnectionBase<Final, Service, BufferSize, Socket>::
handleRead(const system::error_code& error, buffer_type buffer, size_t bytes_transferred) {
	if (error) {
		shutdown();
		return ;
	}

//	::printf("Receiving:%lu bytes, buffer.size=%u\n", bytes_transferred, buffer_.size());
	ZC_ASSERT(buffer.setSize(bytes_transferred));

	if (!buffer_.capacity()) {
		ZC_ASSERT(buffer_.reserve(BufferSize));
	}

	ZC_ASSERT(buffer_.add(buffer));

	/**
	ZC_ASSERT(buffer_index_ <= buffer_.capacity());
	*/

	buffer_type::size_type handle_index = 0;

	do {
		/**
		 * got the bytes that had accepted.
		 * would erase them from receive buffer.
		 */
//		::printf("-->Reading:%u bytes with capacity:%u\n", buffer_.size(), buffer_.capacity()/*, buffer_.data() + sizeof(u32)*/);


		/**
		 * there got data in the buffer, add the new data to the buffer
		 */
		size_t read_bytes = onRead(buffer_.slice(handle_index));

		if (error_) {
			shutdown();
			return ;
		}

		if (!read_bytes)
			break;

		handle_index += read_bytes;

		if (handle_index >= buffer_.size()) {
			break;
		}
	}
	while(true);

	if (handle_index > 0)
		ZC_ASSERT(buffer_.erase(0, handle_index));

	/**
	 * buffer_index_ > 0 means theres still data in the buffer
	 */
	doAsyncRead();
}

template <class Final, class Service, u32 BufferSize, class Socket>
const RemoteConfig& ConnectionBase<Final, Service, BufferSize, Socket>::
getRemoteConfig() {
	if (rc_.port) {
		return rc_;
	}

	try {
		rc_.host = socket_.remote_endpoint().address().to_string();
		rc_.port = socket_.remote_endpoint().port();
	}
	catch(std::exception&) {
		;
	}

	return rc_;
}


}}}


#endif /* CONNECTIONBASEDETAILS_HPP_ */
