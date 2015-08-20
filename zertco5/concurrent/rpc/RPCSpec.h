/*
 * RPCSpec.h
 *
 *  Created on: 2015-2-16
 *      Author: Administrator
 */

#ifndef ZERTCORE_RPCSPEC_H_
#define ZERTCORE_RPCSPEC_H_

#include <pch.h>
#include <object/PoolObject.h>
#include <utils/Singleton.h>
#include <thread/ThreadSingleton.h>
#include <utils/ExpiredObjectMap.h>
#include <thread/ThreadHandlerSet.h>

#include "RPCServer.h"
#include "RPCRouterClient.h"

#include "../ConcurrentState.h"

namespace zertcore { namespace concurrent { namespace rpc {
using namespace zertcore::utils;
using namespace zertcore::object;
using namespace zertcore::concurrent;
}}}

namespace zertcore { namespace concurrent { namespace rpc {

/**
 * for key_type:
 *  Can't start with *, these keys were reserved for system command
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

/**
 * RPCHandlers
 */
class RPCHandlers
{
public:
	typedef unordered_map<key_type, rpc_handler_type>
											rpc_handler_map_type;
	typedef unordered_map<key_type, data_sync_handler_type>
											data_sync_handler_map_type;

	/**
	 * CallbackCell
	 */
	struct CallbackCell
	{
		RemoteConfig			rc;
		key_type				key;
		rpc_callback_type		handler;
		ConcurrentState::ptr	state;
	};
	typedef ExpiredObjectMap<u64, CallbackCell>
											rpc_callback_map_type;

public:
	bool init();
	static bool isSystemKey(const key_type& key);

public:
	static bool getRPCHandler(rpc_handler_type& handler, const key_type& key);
	static bool getDataSyncHandler(data_sync_handler_type& handler, const key_type& key);
	bool popCallbackHandler(rpc_callback_type& handler, ConcurrentState::ptr& state,
			key_type& key, const u64& id);
	bool removeCallbackHandler(const u64& id);

public:
	static bool registerRPCHandler(const key_type& key, const rpc_handler_type& handler);
	static bool registerDataSyncHandler(const key_type& key, const data_sync_handler_type& handler,
			const condition_expr_type& cond = condition_expr_type());
	/**
	 * return msg id
	 */
	u32 registerCallbackHandler(const key_type& key, const rpc_callback_type&,
			ConcurrentState::ptr state, const RemoteConfig&, const tick_type& timeout_ms);

public:
	static bool syncRouter(ConcurrentState::ptr state = ConcurrentState::ptr());
	static RPCRouterClient& router() {return router_;}

private:
	void onTimeout(const u64& id);

private:
	static rpc_handler_map_type	rpc_handler_map_;
	static data_sync_handler_map_type
								data_sync_handler_map_;

private:
	rpc_callback_map_type		rpc_callback_map_;

private:
	u64							msg_id_base_;

protected:
	static RPCRouterClient		router_;
};

/**
 * RPCSpec
 */
class RPCSpec :
		public RPCHandlers,
		public ThreadSingleton<RPCSpec>
{
public:
	enum {
		DEFAULT_CB_TIME_OUT					= 10000, //10 sec
		DEFAULT_TIME_OUT					= 10000,
	};

	enum {
		THREAD_INDEX						= 4,
	};

public:
	struct RCDataCell : public PoolObject<RCDataCell>
	{
		u32						id;
		tid_type				tid;
		key_type				key;
		iarchiver_type			ret_data;

		RPCServerConnection::ptr
								server_conn;
		RPCClientConnection::ptr
								client_conn;

		ZC_TO_STRING(
			"id" << id <<
			"tid" << tid <<
			"key" << key
		);
	};

public:
	static bool initServer(const RemoteConfig& config);
	static bool initClient(const RemoteConfig& config);
	static bool setup();

	virtual void init();

public:
	bool pushRemoteCall(const oarchiver_type& data, RPCServerConnection::ptr conn);
	bool pushRemoteCall(const oarchiver_type& data, RPCClientConnection::ptr conn);

public:
	bool makeAsyncCall(RPCClientConnection::ptr server_conn, const key_type& key,
			iarchiver_type params, const rpc_callback_type& handler, ConcurrentState::ptr state = ConcurrentState::ptr(),
			const tick_type& timeout_ms = DEFAULT_CB_TIME_OUT);
	bool makeCall(RPCClientConnection::ptr server_conn, const key_type& key,
			iarchiver_type, oarchiver_type& oar, const tick_type& timeout_ms = DEFAULT_TIME_OUT);
	bool makeNotify(RPCClientConnection::ptr client_conn, const key_type& key,
			iarchiver_type);

public:
	bool pushCallback(const oarchiver_type& data);
	bool pushDataSync(const oarchiver_type& data);

public:
	RPCHandlers& handlers() {
		return *this;
	}

private:
	void handleRemoteCallResult(RuntimeContext::ptr rc, RCDataCell::ptr cell);

private:
	static RPCServer			server_,
								client_;
};

}}}

#endif /* RPCSPEC_H_ */
