/*
 * IOService.h
 *
 *  Created on: 2015Äê1ÔÂ13ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_IOSERVICE_H_
#define ZERTCORE_IOSERVICE_H_

#include <pch.h>

#include <utils/types.h>
#include <thread/ThreadPool.h>

#include "config.h"

namespace zertcore { namespace net {
using namespace zertcore::concurrent;
}}

namespace zertcore { namespace net {

template <class Final>
class IOService
{
	typedef IOService<Final>				self;
public:
	IOService() :
		enable_ssl_(false), ssl_context_(asio::ssl::context::sslv23) {}

public:
	bool init(const ServiceConfig& config);

public:
	asio::io_service& getIOService() {
		return io_service_;
	}

protected:
	void mainThread() {
		io_service_.run();
	}
	void stop() {
		io_service_.stop();
	}

private:
	string getSSLPassword() const {
		return ssl_password_;
	}

protected:
	bool						enable_ssl_;
	asio::ssl::context			ssl_context_;
	string						ssl_password_;

protected:
	asio::io_service			io_service_;
};

}}

#include "details/IOServiceDetails.hpp"

#endif /* IOSERVICE_H_ */
