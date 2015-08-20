/*
 * PackageConnection.h
 *
 *  Created on: 2015年6月14日
 *      Author: Administrator
 */

#ifndef ZERTCORE_NET_UDP_PACKAGECONNECTION_H_
#define ZERTCORE_NET_UDP_PACKAGECONNECTION_H_

#include "config.h"
#include "ConnectionBase.h"

#include <utils/buffer/SharedBuffer.h>
#include <thread/ThreadHandler.h>

namespace zertcore { namespace net { namespace udp {
using namespace zertcore::utils;
using namespace zertcore::concurrent;
}}}

namespace zertcore { namespace net { namespace udp {

/**
 * PackageConnection<Final, Host>
 */
template <class Final, class Server>
class PackageConnection:
		public ConnectionBase<Final, Server>
{
	typedef ConnectionBase<Final, Server>	self;
public:
	typedef ThreadHandler<void (typename self::ptr, SharedBuffer)>
											package_handler_type;
public:
	explicit PackageConnection(Server& server, peer_ptr peer) :
		ConnectionBase<Final, Server>(server, peer) {}
	virtual ~PackageConnection() {}

public:
	virtual void onPackage(const SharedBuffer& buffer) {;}
	/**
	 * the buffer size must less than 0xfffc
	 */
	bool sendPackage(const SharedBuffer& buffer);

public:
	virtual size_t onRead(const SharedBuffer& buffer) final;

public:
	static void setPackageHandler(const package_handler_type& handler) {
		package_handler_ = handler;
	}

private:
	static package_handler_type	package_handler_;
};

template <class F, class S>
typename PackageConnection<F, S>::package_handler_type
								PackageConnection<F, S>::package_handler_;

}}}


#endif /* NET_UDP_PACKAGECONNECTION_H_ */
