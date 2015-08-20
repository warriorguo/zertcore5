/*
 * IOService.h
 *
 *  Created on: 2015��1��13��
 *      Author: Administrator
 */

#ifndef ZERTCORE_TCP_IOSERVICE_H_
#define ZERTCORE_TCP_IOSERVICE_H_

#include <pch.h>

#include <utils/types.h>
#include <thread/ThreadPool.h>

#include "config.h"
#include "../server_config.h"

namespace zertcore { namespace net { namespace tcp {
using namespace zertcore::concurrent;
}}}

namespace zertcore { namespace net { namespace tcp {

template <class Final>
class IOService
{
	typedef IOService<Final>				self;
public:
	IOService()
#ifdef ZC_ENABLE_SSL
:
		enable_ssl_(false), ssl_context_(asio::ssl::context::sslv23)
#endif
{}
	virtual ~IOService() {}

public:
	bool setup(const ServiceConfig& config);

public:
	asio::io_service& getIOService() {
		return io_service_;
	}

protected:
	size_t mainThread() {
		return io_service_.poll_one();
	}
	void stop() {
		io_service_.stop();
	}

#ifdef ZC_ENABLE_SSL
private:
	string getSSLPassword() const {
		return ssl_password_;
	}

protected:
	bool						enable_ssl_;
	asio::ssl::context			ssl_context_;
	string						ssl_password_;
#endif

protected:
#ifdef ZC_SINGLE_IO_SERVICE
	static
#endif
	asio::io_service			io_service_;
};

#ifdef ZC_SINGLE_IO_SERVICE
template <class Final>
asio::io_service				IOService<Final>::io_service_;
#endif

}}}

#include "details/IOServiceDetails.hpp"

#endif /* IOSERVICE_H_ */
