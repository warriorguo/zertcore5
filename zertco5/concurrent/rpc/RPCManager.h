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
#include "RPCClient.h"

/**
 * for key_type:
 *  Can't start with *, these keys were reserved for system
 *
 * RFC for Request:
 *
 * reqs["id"] -> u32 MessageID unique ID for a request
 * reqs["key"] -> key_type
 * reqs["params"] -> Object
 *
 * for Response:
 * ret["head"]["id"] -> u32
 * optional ret["head"]["err"] -> string
 * optional ret["body"][key1] -> Object1
 * 			ret["body"][key2] -> Object2
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
	typedef unordered_map<key_type, rpc_handler_type>
											rpc_handler_map_type;
	typedef unordered_map<key_type, data_sync_handler_type>
											data_sync_handler_map_type;
	typedef unordered_map<key_type, data_gen_handler_type>
											data_gen_handler_map_type;

	/**
	 * CallbackCell
	 */
	struct CallbackCell
	{
		key_type				key;
		rpc_callback_type		handler;
	};
	typedef unordered_map<u32, CallbackCell>
											rpc_callback_map_type;

public:
	struct RCDataCell : public PoolObject<RCDataCell>
	{
		u32						id;
		key_type				key;
		iachiver_type			ret_data;
		oachiver_type			data;

		RPCServerConnection::ptr
								server_conn;
		RPCClientConnection::ptr
								client_conn;

		ZC_TO_STRING(
			"id" << id <<
			"key" << key
		);
	};

public:
	bool registerHandler(const key_type& key, const rpc_handler_type& handler);
	bool getHandler(rpc_handler_type& handler, const key_type& key);

	bool pushRemoteCall(const oachiver_type& data, RPCServerConnection::ptr conn);
	void handleRemoteCallResult(const RunningContext& rc, RCDataCell::ptr cell);

public:
	bool registerDataSyncHandler(const key_type& key, const data_sync_handler_type& handler);
	bool getDataSyncHandler(data_sync_handler_type& handler, const key_type& key);

	/**
	 * return true if theres data syn handler
	 */
	bool pushDataSynHandler(const oachiver_type& data);
	void handleDataSynResult(const RunningContext& rc, RCDataCell::ptr cell);

public:
	bool registerDataGenHandler(const key_type& key, data_gen_handler_type& handler);
	bool triggerDataGenHandler(const key_type& key);

public:
	void notify(const key_type& key, const iachiver_type&);
	void call(const key_type& key, const iachiver_type&, const rpc_callback_type& handler);

public:
	bool pushCallback(const oachiver_type& data);

private:
	void registerCallbackHandler(const key_type& key, const rpc_callback_type&, iachiver_type&);

private:
	RPCClient					client_;

private:
	rpc_handler_map_type		rpc_handler_map_;
	rpc_callback_map_type		rpc_callback_map_;

	data_sync_handler_map_type	data_sync_handler_map_;
	data_gen_handler_map_type	data_gen_handler_map_;

private:
	u32							msg_id_base_;
};

}}}


#endif /* RPCMANAGER_H_ */
