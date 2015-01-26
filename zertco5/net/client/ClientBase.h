/*
 * ClientBase.h
 *
 *  Created on: 2015Äê1ÔÂ13ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_CLIENTBASE_H_
#define ZERTCORE_CLIENTBASE_H_

#include <pch.h>

#include <utils/types.h>

#include "../IOService.h"
#include "config.h"

namespace zertcore { namespace net {
using namespace zertcore::utils;
}}

namespace zertcore { namespace net { namespace client {

template <class Final, class Connection>
class ClientBase :
		public IOService<Final>
{
	typedef ClientBase<Final, Connection>	self;
public:
	typedef Final							final_type;

	typedef Connection						connection_type;
	typedef typename Connection::ptr		connection_ptr;

public:
	ClientBase() : resolver_(this->template io_service_) {}

public:
	bool init(const ClientConfig& config);
	connection_ptr connect(const string& host, const u32& port);

private:
	void handleResolve(const system::error_code& err,
			asio::ip::tcp::resolver::iterator ep, connection_ptr conn);

private:
	asio::ip::tcp::resolver		resolver_;
};

}}}

#include "details/ClientBaseDetails.hpp"

#endif /* CLIENTBASE_H_ */
