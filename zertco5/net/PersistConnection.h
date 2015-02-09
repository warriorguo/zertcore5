/*
 * PersistConnection.h
 *
 *  Created on: 2015年2月5日
 *      Author: Administrator
 */

#ifndef ZERTCORE_PERSISTCONNECTION_H_
#define ZERTCORE_PERSISTCONNECTION_H_

#include <pch.h>
#include <utils/types.h>
#include <utils/time/Timer.h>

#include "ConnectionBase.h"

namespace zertcore { namespace net {
using namespace zertcore::time;
}}

namespace zertcore { namespace net {

namespace cmd {

enum {
	HEARTBEAT								= 0xfffffff7,
	HEARTBEAT_CALLBACK						= 0xfffffff6,
};

}

/**
 * PersistConnection
 * Just for persist connection that each package size LESS THAN BufferSize
 * transport.
 *
 */
template <class Final, class Service, u32 BufferSize = ZC_CONNECTION_BUFFER_SIZE,
		class Socket = asio::ip::tcp::socket>
class PersistConnection :
		public ConnectionBase<Final, Service, BufferSize, Socket>
{
public:
	const static u32 HEARTBEAT_TIME			= 3000;

public:
	PersistConnection(service_type& service);
	virtual ~PersistConnection() {}

public:
	void enableHearbeat();

public:
	virtual size_t onRead(const SharedBuffer& buffer) final;
	virtual void onPackage(const SharedBuffer& buffer) {;}

public:
	void sendPackage(const SharedBuffer& buffer);

private:
	bool handleCommand(const u32& cmd);

private:
	Timer						expired_timer_;
};

}}

#include "details/PersistConnectionDetails.hpp"

#endif /* PERSISTCONNECTION_H_ */
