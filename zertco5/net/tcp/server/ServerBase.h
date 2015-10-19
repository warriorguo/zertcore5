/*
 * ServerBase.h
 *
 *  Created on: 2015��1��7��
 *      Author: Administrator
 */

#ifndef ZERTCORE_NET_TCP_SERVER_BASE_H_
#define ZERTCORE_NET_TCP_SERVER_BASE_H_

#include <pch.h>

#include <utils/types.h>
#include <object/ObjectTraits.h>

#include "../IOService.h"
#include "config.h"

namespace zertcore { namespace net { namespace tcp {
using namespace zertcore::utils;
using namespace zertcore::object;
}}}

namespace zertcore { namespace net {  namespace tcp { namespace server {
/**
 * Server<Connection>, implement
 */
template <class Final, class Connection>
class ServerBase :
		public IOService<Final>
{
	typedef ServerBase<Final, Connection>	self;

public:
	typedef Final							final_type;

	typedef Connection						connection_type;
	typedef typename ObjectTraits<Connection>::ptr
											connection_ptr;

public:
	ServerBase() : is_running_(false),
		acceptor_(this->template io_service_) {}
	virtual ~ServerBase() {}

public:
	bool setup(const ServerConfig& config);

protected:
	virtual void onStart();

private:
	void startAccept();
	void continueAccept();

private:
	void handleAccept(const system::error_code& e, connection_ptr conn);

private:
	bool						is_running_;
	ServerConfig				config_;

private:
	asio::ip::tcp::acceptor		acceptor_;
};

}}}}

#include "details/ServerBaseDetails.hpp"

#endif /* SERVER_H_ */
