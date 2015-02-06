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

#include "ConnectionBase.h"

namespace zertcore { namespace net {

namespace cmd {

enum {
	HEARTBEAT								= 0xfffffff7,
	HEARTBEAT_CALLBACK						= 0xfffffff6,
};

}

asio::basic_deadline_timer;

template <class Final, class Service, u32 BufferSize = ZC_CONNECTION_BUFFER_SIZE,
		class Socket = asio::ip::tcp::socket>
class PersistConnection :
		public ConnectionBase<Final, Service, BufferSize, Socket>
{
public:
	virtual ~PersistConnection() {}

public:
	void enableHearbeat();

public:
	virtual size_t onRead(const SharedBuffer& buffer);
	virtual void onPackage(const SharedBuffer& buffer) {;}

private:
	bool handleCommand(const u32& cmd);

private:
};

}}

#include "details/PersistConnectionDetails.hpp"

#endif /* PERSISTCONNECTION_H_ */
