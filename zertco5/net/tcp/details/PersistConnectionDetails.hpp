/*
 * PersistConnectionDetails.hpp
 *
 *  Created on: 2015年2月5日
 *      Author: Administrator
 */

#ifndef ZERTCORE_TCP_PERSISTCONNECTIONDETAILS_HPP_
#define ZERTCORE_TCP_PERSISTCONNECTIONDETAILS_HPP_

#include <log/Log.h>

namespace zertcore { namespace net { namespace tcp {

template <class Final, class Service, u32 BufferSize, class Socket>
PersistConnection<Final, Service, BufferSize, Socket>::
PersistConnection(Service& service) :
	ConnectionBase<Final, Service, BufferSize, Socket>(service), time_out_try_times_(0) {
	;
}

template <class Final, class Service, u32 BufferSize, class Socket>
void PersistConnection<Final, Service, BufferSize, Socket>::
setTimeoutHandler(const timeout_handler_type& handler) {
	timeout_handler_ = handler;
}

template <class Final, class Service, u32 BufferSize, class Socket>
void PersistConnection<Final, Service, BufferSize, Socket>::
setPackageHandler(const package_handler_type& handler) {
	package_handler_ = handler;
}

template <class Final, class Service, u32 BufferSize, class Socket>
void PersistConnection<Final, Service, BufferSize, Socket>::
enableHeartbeat(const tick_type& interval) {
	if (!interval) {
		expired_timer_.stop();
		return ;
	}

	expired_timer_.onExpired(bind(&self::handleTimeout, this->template thisPtr(), _1));
	expired_timer_.start(interval);
}

template <class Final, class Service, u32 BufferSize, class Socket>
size_t PersistConnection<Final, Service, BufferSize, Socket>::
onRead(const SharedBuffer& buffer) {
	if (expired_timer_.isWorking()) {
		expired_timer_.reset();
		time_out_try_times_ = 0;
	}

	if (buffer.size() < sizeof(u32)) {
		ZCLOG(ERROR) << "Buffer was too small, size=" << buffer.size() << End;
		return 0;
	}

	u32 length = *((const u32 *)buffer.data());
	if (length > BufferSize) {
		if (handleCommand(length))
			return sizeof(u32);

		ZCLOG(ERROR) >> this->template error() << "Length was too large:" << length << End;
		buffer.print();
		return 0;
	}

	if (length > buffer.size()) {
		ZCLOG(NOTE) << "length=" << length << ",size=" << buffer.size() << End;
		return 0;
	}

	SharedBuffer pkg = buffer.slice((u32)sizeof(u32), length - sizeof(u32));
	onPackage(pkg);

	if (package_handler_) {
		package_handler_.setParams(this->template thisPtr(), pkg);
		package_handler_.push();
	}

	return length;
}

template <class Final, class Service, u32 BufferSize, class Socket>
bool PersistConnection<Final, Service, BufferSize, Socket>::
sendHeartBeatCommand() {
	static SharedBuffer heart_beat_buf;
	if (heart_beat_buf.empty()) {
		u32 c = cmd::HEARTBEAT;
		heart_beat_buf.assign(&c, sizeof(c));
	}
	return this->template write(heart_beat_buf);
}

template <class Final, class Service, u32 BufferSize, class Socket>
bool PersistConnection<Final, Service, BufferSize, Socket>::
sendPackage(const SharedBuffer& buffer, SharedBuffer& receive_buffer) {
	u32 head_size = buffer.size() + sizeof(u32);

	SharedBuffer send_buf(buffer.size() + sizeof(u32));
	send_buf.add((const u8 *)&head_size, sizeof(u32));
	send_buf.add(buffer);

	if (!this->template write(send_buf))
		return false;

	while(true) {
		u32 head_size = 0;
		this->template read((u8 *)&head_size, sizeof(head_size));
		if (head_size >= BufferSize) {
			if (handleCommand(head_size, BLOCK))
				continue;

			this->template shutdown();
			return false;
		}
		else if (head_size < sizeof(u32)) {
			this->template shutdown();
			return false;
		}

		receive_buffer.resize(head_size - sizeof(u32));
		if (head_size != this->template read(receive_buffer)) {
			return false;
		}

		break;
	}

	return true;
}

template <class Final, class Service, u32 BufferSize, class Socket>
bool PersistConnection<Final, Service, BufferSize, Socket>::
sendPackage(const SharedBuffer& buffer) {
	u32 head_size = buffer.size() + sizeof(u32);

	SharedBuffer send_buf(buffer.size() + sizeof(u32));
	ZC_ASSERT(send_buf.add((const u8 *)&head_size, sizeof(u32)));
	ZC_ASSERT(send_buf.add(buffer));

//	this->template asyncWrite(send_buf);
	if (!this->template write(send_buf))
		return false;

	return this->template asyncRead();
}

template <class Final, class Service, u32 BufferSize, class Socket>
bool PersistConnection<Final, Service, BufferSize, Socket>::
asyncSendPackage(const SharedBuffer& buffer) {
	u32 head_size = buffer.size() + sizeof(u32);

	SharedBuffer send_buf(buffer.size() + sizeof(u32));
	ZC_ASSERT(send_buf.add((const u8 *)&head_size, sizeof(u32)));
	ZC_ASSERT(send_buf.add(buffer));

	return this->template asyncWrite(send_buf);
}

template <class Final, class Service, u32 BufferSize, class Socket>
bool PersistConnection<Final, Service, BufferSize, Socket>::
handleCommand(const u32& cmd, const block_type& block) {
	static SharedBuffer heart_beat_cb_buf;
	if (heart_beat_cb_buf.empty()) {
		u32 c = cmd::HEARTBEAT_CALLBACK;
		heart_beat_cb_buf.assign(&c, sizeof(c));
	}
	switch(cmd) {
	case cmd::HEARTBEAT: {
		this->template write(heart_beat_cb_buf);
		break;
	}

	case cmd::HEARTBEAT_CALLBACK:
		// ignore this command since expired timer would refresh in onRead()
		break;

	default:
		return false;
	}

	return true;
}

template <class Final, class Service, u32 BufferSize, class Socket>
void PersistConnection<Final, Service, BufferSize, Socket>::
handleTimeout(const timer_type::counter_type& interval) {
	++time_out_try_times_;

	if (time_out_try_times_ < TIMEOUT_TRY_TIMES) {
		if (sendHeartBeatCommand()) {
			expired_timer_.reset();
			return ;
		}
	}

	if (timeout_handler_) {
		timeout_handler_.push(this->template thisPtr());
	}

	expired_timer_.stop();
	onTimeout();
}

}}}


#endif /* PERSISTCONNECTIONDETAILS_HPP_ */
