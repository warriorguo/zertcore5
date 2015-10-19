/*
 * PersistConnection.h
 *
 *  Created on: 2015年2月5日
 *      Author: Administrator
 */

#ifndef ZERTCORE_TCP_PERSISTCONNECTION_H_
#define ZERTCORE_TCP_PERSISTCONNECTION_H_

#include <pch.h>
#include <utils/types.h>
#include <utils/time/Timer.h>
#include <thread/ThreadHandler.h>

#include "ConnectionBase.h"

namespace zertcore { namespace net { namespace tcp {
using namespace zertcore::concurrent;
}}}

namespace zertcore { namespace net { namespace tcp {

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
	typedef PersistConnection				self;
public:
	enum {
		TIMEOUT_TRY_TIMES					= 3,
	};

public:
	typedef ThreadHandler<void (typename self::ptr)>
											timeout_handler_type;
	typedef ThreadHandler<void (typename self::ptr, SharedBuffer)>
											package_handler_type;

public:
	PersistConnection(Service& service);
	virtual ~PersistConnection() {}

public:
	static void setTimeoutHandler(const timeout_handler_type& handler);
	static void setPackageHandler(const package_handler_type& handler);

public:
	void enableHeartbeat(const tick_type& interval);

public:
	virtual size_t onRead(const SharedBuffer& buffer) final;

public:
	/**
	 * IMPORTANT: these two methods were RUNNING in the IO handling thread!!
	 */
	virtual void onPackage(const SharedBuffer& buffer) {;}
	virtual void onTimeout() {this->template shutdown();}

	bool sendHeartBeatCommand();

public:
	bool sendPackage(const SharedBuffer& buffer, SharedBuffer& receive_buffer);
	bool sendPackage(const SharedBuffer& buffer);
	/**
	 * IMPORTANT!!!
	 * when the network was too bad or the frequency was too fast
	 * the async write got problem:
	 * the package would not receive by the order by send queue.
	 * at this situation, its better to use sendPackage(buffer)
	 */
	bool asyncSendPackage(const SharedBuffer& buffer);

private:
	bool handleCommand(const u32& cmd, const block_type& block = NON_BLOCK);
	void handleTimeout(const timer_type::counter_type& interval);

private:
	timer_type					expired_timer_;

private:
	u32							time_out_try_times_;

private:
	static timeout_handler_type	timeout_handler_;
	static package_handler_type	package_handler_;
};

/**
 * static
 */
template <class Final, class Service, u32 BufferSize, class Socket>
typename PersistConnection<Final, Service, BufferSize, Socket>::timeout_handler_type
								PersistConnection<Final, Service, BufferSize, Socket>::timeout_handler_;

template <class Final, class Service, u32 BufferSize, class Socket>
typename PersistConnection<Final, Service, BufferSize, Socket>::package_handler_type
								PersistConnection<Final, Service, BufferSize, Socket>::package_handler_;

}}}

#include "details/PersistConnectionDetails.hpp"

#endif /* PERSISTCONNECTION_H_ */
