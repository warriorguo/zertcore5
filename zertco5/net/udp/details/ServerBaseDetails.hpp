/*
 * ServerBaseDetails.hpp
 *
 *  Created on: 2015年6月14日
 *      Author: Administrator
 */

#ifndef ZERTCORE_NET_UDP_DETAILS_SERVERBASEDETAILS_HPP_
#define ZERTCORE_NET_UDP_DETAILS_SERVERBASEDETAILS_HPP_

#include "../ServerBase.h"

#include <log/Log.h>
#include <thread/ThreadPool.h>

namespace zertcore { namespace net { namespace udp {
using namespace zertcore::concurrent;
}}}

namespace zertcore { namespace net { namespace udp {

template <class F, class C>
ServerBase<F, C>::
~ServerBase() {
	if (host_) {
		enet_host_destroy(host_);
		host_ = nullptr;
	}
}

template <class F, class C>
bool ServerBase<F, C>::
setup(const ServerConfig& rc) {
	ENetAddress addr;
	enet_address_set_host(&addr, rc.host.c_str());
	addr.port = rc.port;

	host_ = enet_host_create(&addr, rc.peer_size, rc.channel_size, 0, 0);
	if (!host_) {
		ZCLOG(ERROR) << "enet_host_create() failed" << End;
		return false;
	}

	if (!ThreadPool::Instance().
			registerExclusiveHandler(bind(&self::mainThread, this),
					bind(&self::startRun, this))) {
		ZCLOG(ERROR) << "ThreadPool::registerExclusiveHandler() failed" << End;
		return false;
	}

	return true;
}

template <class F, class C>
bool ServerBase<F, C>::
flush() {
	return true;
	/**
	 * comment these code since they would run in other thread!
	 *
	ZC_DEBUG_ASSERT( host_ );
	enet_host_flush(host_);
	return true;
	*/
}

template <class F, class C>
void ServerBase<F, C>::
startRun() {
	;
}

template <class F, class C>
size_t ServerBase<F, C>::
mainThread() {
	ZC_DEBUG_ASSERT( host_ );

	size_t handle_size = 0;
	ENetEvent event;

	while (enet_host_service(host_, &event, 50) > 0) {
		switch (event.type) {
		case ENET_EVENT_TYPE_CONNECT: {
			connection_ptr conn = connection_type::template create(static_cast<final_type&>(*this), event.peer);
			event.peer->data = conn.get();
			connection_map_[conn.get()] = conn;

			conn->handleConnect();
			break;
		}

		case ENET_EVENT_TYPE_RECEIVE: {
			connection_pure_ptr conn = (connection_pure_ptr)event.peer->data;
			ZC_DEBUG_ASSERT( connection_map_.find(conn) != connection_map_.end() );

			conn->handleRead((const u8 *)event.packet->data, event.packet->dataLength);

			break;
		}

		case ENET_EVENT_TYPE_DISCONNECT: {
			connection_pure_ptr conn = (connection_pure_ptr)event.peer->data;
			/**
			 * in close method, any thread handler operation should save the ptr of conn self
			 */
			conn->handleShutdown();

			ZC_DEBUG_ASSERT( connection_map_.erase(conn) );
			break;
		}

		default:
			break;
		}

		handle_size++;
	}

	return handle_size;
}

}}}


#endif /* NET_UDP_DETAILS_SERVERBASEDETAILS_HPP_ */
