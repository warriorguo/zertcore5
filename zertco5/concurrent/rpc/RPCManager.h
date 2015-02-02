/*
 * RPCManager.h
 *
 *  Created on: 2015年2月1日
 *      Author: Administrator
 */

#ifndef ZERTCORE_RPCMANAGER_H_
#define ZERTCORE_RPCMANAGER_H_

#include <utils/Singleton.h>

#include "config.h"
#include "RPCConnection.h"

namespace zertcore { namespace concurrent { namespace rpc {
using namespace zertcore::utils;
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

public:
	bool registerHandler(const key_type& key, const rpc_handler_type& handler);
	bool getHandler(rpc_handler_type& handler, const key_type& key);

	bool putRemoteCall(const oachiver_type& params, iachiver_type& ret_data, RPCServerConnection::ptr conn);
	void handleRemoteCallResult(const RunningContext& rc, iachiver_type& ret_data);

public:
	bool registerDataSyncHandler(const key_type& key, const data_sync_handler_type& handler);
	bool getDataSyncHandler(data_sync_handler_type& handler, const key_type& key);

public:
	void call(const key_type& key, const iachiver_type&);
	void call(const key_type& key, const iachiver_type&, const callback_handler_type& handler);

private:
	rpc_handler_map_type		rpc_handler_map_;
	data_sync_handler_map_type	data_sync_handler_map_;
};

}}}


#endif /* RPCMANAGER_H_ */
