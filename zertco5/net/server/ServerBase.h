/*
 * ServerBase.h
 *
 *  Created on: 2015��1��7��
 *      Author: Administrator
 */

#ifndef ZERTCORE_SERVERServerBase_H_
#define ZERTCORE_SERVER_BASE_H_

#include <pch.h>

#include <utils/types.h>

#include "../IOService.h"
#include "config.h"

namespace zertcore { namespace net {
using namespace zertcore::utils;
}}

namespace zertcore { namespace net { namespace server {
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
	typedef typename Connection::ptr		connection_ptr;

public:
	ServerBase() : is_running_(false), thread_size_(0),
		acceptor_(this->template io_service_) {}

public:
	bool setupServer(const ServerConfig& config);

private:
	void startAccept(const u32& amount);
	void continueAccept();

private:
	void handleAccept(const system::error_code& e, connection_ptr conn);

private:
	bool						is_running_;
	u32							thread_size_;

private:
	asio::ip::tcp::acceptor		acceptor_;
};

}}}

#include "details/ServerBaseDetails.hpp"

#endif /* SERVER_H_ */
