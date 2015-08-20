/*
 * RPCDataProvider.h
 *
 *  Created on: 2015年4月14日
 *      Author: Administrator
 */

#ifndef ZERCORE_CONCURRENT_RPC_DETAILS_RPCDATAPROVIDER_H_
#define ZERCORE_CONCURRENT_RPC_DETAILS_RPCDATAPROVIDER_H_

#include <pch.h>

#include <db/io/DataProvider.h>
#include <db/Database.h>

#include "../RPCManager.h"

namespace zertcore { namespace db { namespace io {
using namespace zertcore::concurrent::rpc;

namespace details {
enum {
	RUNNING_THREAD_ID						= 5,
};
}

}}}


namespace zertcore { namespace db { namespace io {

/**
 * RPCDataProvider<Object>
 *
 * this implementation was just for client side.
 * the purpose of RPC was working with multi-machines & multi-languages,
 * so could write the server side in C++ or other languages.
 */
template <class Object>
class RPCDataProvider :
		public DataProvider<Object>
{
	ZC_TO_STRING("RPCDataProvider");
public:
	virtual ~RPCDataProvider() {}

public:
	virtual bool serialize(object_ptr object, const id_type& id) {
		iachiver_type ar;
		ar & *object;
/**
 * FUCCCCCCCCCK!!! Need to enable the block mode in connection!!!
 */
		return RPCManager::Instance().notify(ActiveObjectTraits<Object>::RPC_NAME, ar);
	}
	virtual bool unserialize(object_ptr object, const id_type& id) {
		iachiver_type iar;
		iar["id"] & id;

		oachiver_type oar;
		if (!RPCManager::Instance().call(ActiveObjectTraits<Object>::RPC_NAME, iar, oar, id))
			return false;

		return oar & *object;
	}

public:
	virtual u32 getThreadIndex() const {
		return details::RUNNING_THREAD_ID;
	}
};

}}}


#endif /* CONCURRENT_RPC_DETAILS_RPCDATAPROVIDER_H_ */
