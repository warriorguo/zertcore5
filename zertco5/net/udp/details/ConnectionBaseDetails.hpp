/*
 * ConnectionBaseDetails.hpp
 *
 *  Created on: 2015年6月13日
 *      Author: Administrator
 */

#ifndef ZERTCORE_NET_UDP_DETAILS_CONNECTIONBASEDETAILS_HPP_
#define ZERTCORE_NET_UDP_DETAILS_CONNECTIONBASEDETAILS_HPP_

#include "../ConnectionBase.h"

#include <object/PoolObject.h>
#include <log/Log.h>

namespace zertcore { namespace net { namespace udp {

/**
template <class F, class S>
bool ConnectionBase<F, S>::
connect(resolver_type ep) {
	return false;
}

template <class F, class S>
bool ConnectionBase<F, S>::
reconnect() {
	return false;
}
*/

template <class F, class S>
void ConnectionBase<F, S>::
shutdown() {
	ZC_ASSERT( peer_ );
	enet_peer_disconnect(peer_, 0);
}

template <class F, class S>
bool ConnectionBase<F, S>::
write(const u8* buffer, size_t size) {
	ENetPacket* packet = enet_packet_create(buffer, size, ENET_PACKET_FLAG_RELIABLE);
	if (!packet) {
		ZCLOG(ERROR) << "enet_packet_create failed" << End;
		return false;
	}

	do {
		spinlock_guard_type guard(server_.getLock());

		if (enet_peer_send(peer_, 0, packet) != 0) {
			ZCLOG(NOTICE) << "enet_peer_send failed" << End;
			return false;
		}
	}
	while(false);

	return server_.template flush();
}

template <class F, class S>
bool ConnectionBase<F, S>::
write(const SharedBuffer& buffer) {
	return write(buffer.data(), buffer.size());
}

template <class F, class S>
void ConnectionBase<F, S>::
handleConnect() {
	onConnect();
}

template <class F, class S>
void ConnectionBase<F, S>::
handleShutdown() {
	::printf("shutdown() IP:%s[%u]\n", getRemoteConfig().host.c_str(), getRemoteConfig().port);

	if (read_buffer_.writable())
		read_buffer_.setSize(0);

	onShutdown();

	if (shutdown_handler_) {
		shutdown_handler_.setParams(this->template thisPtr());
		shutdown_handler_.push();
	}
}

template <class F, class S>
const RemoteConfig& ConnectionBase<F, S>::
getRemoteConfig() const {
	ZC_ASSERT( peer_ );

	if (rc_)
		return rc_;

	/**
	 * inet_ntoa was not thread safe, manually set it up
	 */
	std::stringstream ss;
	ss << ((peer_->address.host & 0xf   ) >> 0) << "."
	   << ((peer_->address.host & 0xf0  ) >> 4) << "."
	   << ((peer_->address.host & 0xf00 ) >> 8) << "."
	   << ((peer_->address.host & 0xf000) >> 12);

	rc_.host = ss.str();
	rc_.port = peer_->address.port;

	return rc_;
}

template <class F, class S>
void ConnectionBase<F, S>::
handleRead(const u8* buffer, size_t size) {
	ZC_ASSERT( peer_ );

	if (!read_buffer_.add(buffer, size)) {
		ZCLOG(ERROR) << "read_buffer add failed" << End;
		return ;
	}

	buffer_type::size_type handle_index = 0;

	do {
		/**
		 * got the bytes that had accepted.
		 * would erase them from receive buffer.
		 */
		::printf("-->Reading:%u bytes with capacity:%u\n", read_buffer_.size(), read_buffer_.capacity());

		/**
		 * there got data in the buffer, add the new data to the buffer
		 */
		size_t read_bytes = onRead(read_buffer_.slice(handle_index));

		if (error_) {
			shutdown();
			return ;
		}

		if (!read_bytes)
			break;

		handle_index += read_bytes;

		if (handle_index >= read_buffer_.size()) {
			break;
		}
	}
	while(true);

	if (handle_index > 0)
		ZC_ASSERT(read_buffer_.erase(0, handle_index));
}

}}}

#endif /* NET_UDP_DETAILS_CONNECTIONBASEDETAILS_HPP_ */
