/*
 * PersistConnectionDetails.hpp
 *
 *  Created on: 2015年2月5日
 *      Author: Administrator
 */

#ifndef PERSISTCONNECTIONDETAILS_HPP_
#define PERSISTCONNECTIONDETAILS_HPP_

namespace zertcore { namespace net {

template <class Final, class Service, u32 BufferSize, class Socket>
void PersistConnection<Final, Service, BufferSize, Socket>::
enableHearbeat() {
	expired_timer_.start(HEARTBEAT_TIME);
}

template <class Final, class Service, u32 BufferSize, class Socket>
size_t PersistConnection<Final, Service, BufferSize, Socket>::
onRead(const SharedBuffer& buffer) {
	if (expired_timer_.isWorking())
		expired_timer_.reset();

	if (buffer.size() < sizeof(u32))
		return 0;

	u32 length = *((const u32 *)buffer.data());
	if (length > BufferSize) {
		if (handleCommand(length))
			return sizeof(u32);

		ZCLOG(ERROR) >> error() << "Length was too large:" << length << End;
		return 0;
	}

	if (length > buffer.size())
		return 0;

	onPackage(buffer.slice(0, length));
	return length;
}

template <class Final, class Service, u32 BufferSize, class Socket>
bool PersistConnection<Final, Service, BufferSize, Socket>::
handleCommand(const u32& cmd) {
	switch(cmd) {
	case HEARTBEAT:
		u32 c = HEARTBEAT_CALLBACK;
		write((const u8*)&c, sizeof(c));
		break;

	case HEARTBEAT_CALLBACK:
		// ignore this command since expired timer would refresh in onRead()
		break;

	default:
		return false;
	}

	return true;
}

}}


#endif /* PERSISTCONNECTIONDETAILS_HPP_ */
