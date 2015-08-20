/*
 * PackageConnectionDetails.hpp
 *
 *  Created on: 2015年6月14日
 *      Author: Administrator
 */

#ifndef NET_UDP_DETAILS_PACKAGECONNECTIONDETAILS_HPP_
#define NET_UDP_DETAILS_PACKAGECONNECTIONDETAILS_HPP_

#include "../PackageConnection.h"

namespace zertcore { namespace net { namespace udp {

template <class F, class S>
size_t PackageConnection<F, S>::
onRead(const SharedBuffer& buffer) {
	if (buffer.size() < sizeof(u16)) {
		ZCLOG(ERROR) << "Buffer was too small, size=" << buffer.size() << End;
		return 0;
	}

	u16 length = *((const u16 *)buffer.data());
	if (length > SharedBuffer::MAX_SIZE) {
		ZCLOG(ERROR) >> this->template error() << "Length was too large:" << length << End;
		buffer.print();
		return 0;
	}

	if (length > buffer.size()) {
		ZCLOG(NOTE) << "length=" << length << ",size=" << buffer.size() << End;
		return 0;
	}

	SharedBuffer pkg = buffer.slice((u32)sizeof(u16), length - sizeof(u16));
	onPackage(pkg);

	if (package_handler_) {
		package_handler_.setParams(this->template thisPtr(), pkg);
		package_handler_.push();
	}

	return length;
}

template <class F, class S>
bool PackageConnection<F, S>::
sendPackage(const SharedBuffer& buffer) {
	if (buffer.size() >= max_u16 - sizeof(u16))
		return false;

	u16 head_size = buffer.size() + sizeof(u16);

	SharedBuffer send_buf(buffer.size() + sizeof(u16));
	ZC_ASSERT(send_buf.add((const u8 *)&head_size, sizeof(u16)));
	ZC_ASSERT(send_buf.add(buffer));

	return this->template write(send_buf);
}

}}}


#endif /* NET_UDP_DETAILS_PACKAGECONNECTIONDETAILS_HPP_ */
