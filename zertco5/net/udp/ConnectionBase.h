/*
 * ConnectionBase.h
 *
 *  Created on: 2015年6月13日
 *      Author: Administrator
 */

#ifndef ZERTCORE_NET_UDP_CONNECTIONBASE_H_
#define ZERTCORE_NET_UDP_CONNECTIONBASE_H_

#include <pch.h>
#include <utils/types.h>
#include <utils/buffer/SharedBuffer.h>
#include <thread/ThreadHandler.h>

#include "config.h"

namespace zertcore { namespace net { namespace udp {
using namespace zertcore::object;
using namespace zertcore::utils;
}}}

namespace zertcore { namespace net { namespace udp {

/**
 * ConnectionBase
 */
template <class Final, class Server>
class ConnectionBase :
		public PoolObject<Final>
{
	typedef ConnectionBase<Final, Server>	self;
public:
	typedef ThreadHandler<void (typename self::ptr)>
											shutdown_handler_type;

public:
	typedef Final*							pure_ptr;
	typedef Server							server_type;

	typedef SharedBuffer					buffer_type;

public:
	explicit ConnectionBase(server_type& server, peer_ptr peer) :
		server_(server), /**is_reliable_(true),*/ peer_(peer) {}
	virtual ~ConnectionBase() {}
/**
public:
	bool connect(resolver_type ep);
	bool reconnect();
*/
public:
	void shutdown();

public:
	const RemoteConfig& getRemoteConfig() const;

protected:
	/**
	 * set the error( e.g. ZCLOG(ERROR) >> error() << "some thing wrong here" << End; ) to shutdown the connection
	 * return the bytes that had accepted.
	 */
	virtual size_t onRead(const SharedBuffer& buffer) {return 0;}
	virtual void onConnect() {}

	virtual void onShutdown() {return;}

public:
	bool write(const u8* buffer, size_t size);
	bool write(const SharedBuffer& buffer);

public:
	/**
	 * just support reliable UDP transaction right now
	 */
	bool isReliable() const {return true;}

public:
	/**
	 * for host using
	 */
	void handleConnect();
	void handleShutdown();
	void handleRead(const u8* buffer, size_t size);
	peer_ptr& peer() {return peer_;}

public:
	Error& error() {return error_;}

public:
	static void setShutdownHandler(const shutdown_handler_type& handler) {
		shutdown_handler_ = handler;
	}

protected:
	server_type&				server_;

protected:
//	bool						is_reliable_;
	mutable RemoteConfig		rc_;

protected:
	Error						error_;
	buffer_type					read_buffer_;

	peer_ptr					peer_;

private:
	static shutdown_handler_type
								shutdown_handler_;
};

template <class F, class S>
typename ConnectionBase<F, S>::shutdown_handler_type
								ConnectionBase<F, S>::shutdown_handler_;

}}}


#endif /* NET_UDP_CONNECTIONBASE_H_ */
