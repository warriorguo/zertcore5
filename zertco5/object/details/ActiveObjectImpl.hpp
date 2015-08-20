/*
 * ActiveObjectImpl.hpp
 *
 *  Created on: 2015年7月22日
 *      Author: Administrator
 */

#ifndef ZERTCORE_OBJECT_DETAILS_ACTIVEOBJECTIMPL_HPP_
#define ZERTCORE_OBJECT_DETAILS_ACTIVEOBJECTIMPL_HPP_

#include "../ActiveObject.h"
#include <concurrent/rpc/RPC.h>

namespace zertcore { namespace object {

template <class Final>
bool ActiveObject<Final>::
setMaster() {
	if (!ActiveObjectTraits<Final>::SYNC_NAME) {
		ZCLOG(ERROR) << "ActiveTraits SYNC_NAME is NULL" << End;
		return false;
	}

	role_ |= ROLE_MASTER;
	return true;
}

template <class Final>
bool ActiveObject<Final>::
setSlave() {
	if (!id_) return false;

	role_ |= ROLE_SLAVE;
	getManager().setupSlave(id_, this->template thisPtr());

	return true;
}

template <class Final>
bool ActiveObject<Final>::
sync() {
	if ( !(role_ & ROLE_MASTER) && !setMaster() ) {
		ZCLOG(NOTE) << "try to sync others not in master role" << End;
		return false;
	}

	return RPC.notify(ActiveObjectTraits<Final>::SYNC_NAME, *this);
}

}}


#endif /* OBJECT_DETAILS_ACTIVEOBJECTIMPL_HPP_ */
