/*
 * RPCManager.h
 *
 *  Created on: 2015年2月1日
 *      Author: Administrator
 */

#ifndef ZERTCORE_RPCMANAGER_H_
#define ZERTCORE_RPCMANAGER_H_

#include <utils/Singleton.h>
#include <object/PoolObject.h>

#include "config.h"
#include "RPCConnection.h"

/**
 * RFC for Request:
 *
 * reqs["id"] -> u32 MessageID unique ID for a request
 * reqs["key"] -> key_type
 * reqs["params"] -> Object
 *
 *
 *
 */

namespace zertcore { namespace concurrent { namespace rpc {
using namespace zertcore::utils;
using namespace zertcore::object;
}}}

namespace zertcore { namespace concurrent { namespace rpc {

/**
 * RPCManager
 */
class RPCManager :
		public Singleton<RPCManager>
{
public:
	typedef map<key_type, rpc_handler_type>	rpc_handler_map_type;
	typedef map<key_type, data_sync_handler_type>
											data_sync_handler_map_type;

	struct RCDataCell : public PoolObject<RCDataCell>
	{
		u32						id;
		key_type				key;
		iachiver_type			ret_data;
		oachiver_type			params;

		RPCServerConnection::ptr
								conn;

		ZC_TO_STRING("id" << id << "key" << key);
	};

public:
	bool registerHandler(const key_type& key, const rpc_handler_type& handler);
	bool getHandler(rpc_handler_type& handler, const key_type& key);

	bool putRemoteCall(const oachiver_type& data, RPCServerConnection::ptr conn);
	void handleRemoteCallResult(const RunningContext& rc, RCDataCell::ptr cell);

public:
	bool registerDataSyncHandler(const key_type& key, const data_sync_handler_type& handler);
	bool getDataSyncHandler(data_sync_handler_type& handler, const key_type& key);

public:
	void call(const key_type& key, const iachiver_type&);
	void call(const key_type& key, const iachiver_type&, const rpc_callback_type& handler);

private:
	rpc_handler_map_type		rpc_handler_map_;
	data_sync_handler_map_type	data_sync_handler_map_;
};

}}}


#endif /* RPCMANAGER_H_ */
