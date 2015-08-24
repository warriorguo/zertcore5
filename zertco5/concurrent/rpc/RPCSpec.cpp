/*
 * RPCSpec.cpp
 *
 *  Created on: 2015-2-16
 *      Author: Administrator
 */

#include "RPCSpec.h"
#include "RPCClient.h"

#include <concurrent/Concurrent.h>
#include <log/Log.h>
#include <core/Runtime.h>
#include <details.h>

namespace zertcore { namespace concurrent { namespace rpc {

RPCServer						RPCSpec::server_,
								RPCSpec::client_;

}}}

namespace zertcore { namespace concurrent { namespace rpc {

bool RPCSpec::
initServer(const RemoteConfig& config) {
	ServerConfig svr;
	svr = config;

	svr.accept_nums = 1;
	svr.thread_nums = 1;

	router_.serverConfig().rc = config;

	server_.setType(RPCServer::TYPE_SERVER);
	return server_.setup(svr);
}

bool RPCSpec::
initClient(const RemoteConfig& config) {
	ServerConfig svr;
	svr = config;

	svr.accept_nums = 1;
	svr.thread_nums = 1;

	router_.clientConfig().rc = config;

	client_.setType(RPCServer::TYPE_DATASYNC);
	return client_.setup(svr);
}

void RPCSpec::
init() {
	ZC_ASSERT( RPCHandlers::init() );
}

bool RPCSpec::
setup() {
	RT.initHandler([] () {

	//

	/**
	 * set up client package handler
	 */
	RPCClientConnection::setPackageHandler([] (RPCClientConnection::ptr conn, SharedBuffer buffer) {
		oarchiver_type o;
		if (!o.buffer(buffer)) {
			ZCLOG(ERROR) >> conn->error() << "Parsing data from [" << conn->getRemoteConfig().toString()
					<< "] failed, turn it down." << End;
#ifndef ZC_RELEASE
			::printf("RAW DATA:%s\n", buffer.data());
			ZC_DEBUG_ASSERT(false);
#endif
			return ;
		}

		/**
		 * get the tid and push it to the right thread
		 */
		tid_type tid = 0;
		ZC_ASSERT(o["tid"] & tid);

		if (!goThread(tid, [o, conn]() {
			if (!RPCSpec::Instance().pushRemoteCall(o, conn)) {
				ZCLOG(NOTICE) << "pushRemoteCall Failed:" << End;
			}
		})) {
			ZCLOG(ERROR) >> conn->error() << "goThread Failed" << End;
		}
	});

	/**
	 * set up client package handler
	 */
	RPCClientConnection::setTimeoutHandler([] (RPCClientConnection::ptr conn) {
		;
	});

	/**
	 * set up server package handler
	 */
	RPCServerConnection::setPackageHandler([] (RPCServerConnection::ptr conn, SharedBuffer buffer) {
		oarchiver_type o;

		if (!o.buffer(buffer)) {
			ZCLOG(ERROR) >> conn->error() << "Parsing data from [" << conn->getRemoteConfig().toString()
					<< "] failed, turn it down." << End;
#ifndef ZC_RELEASE
			::printf("RAW DATA:[%u]%s\n", buffer.size(), buffer.data());
			ZC_DEBUG_ASSERT(false);
#endif
			return ;
		}

/**
 * specify the tid make it complex but worthless
 *
		tid_type tid = 0;
		ZC_ASSERT(o["tid"] & tid);
		if (!goThread(tid, [&]() {
*/
			if (conn->getService().isDataSync()) {
//				RPCSpec::Instance().push(&RPCSpec::pushDataSync, o);
				if (!RPCSpec::Instance().pushDataSync(o)) {
					ZCLOG(WARNING) << "pushDataSynHandler Failed" << End;
				}
			}
			else {
//				RPCSpec::Instance().push(&RPCSpec::pushRemoteCall, o, conn);
				if (!RPCSpec::Instance().pushRemoteCall(o, conn)) {
					ZCLOG(WARNING) << "pushRemoteCall Failed" << End;
				}
			}
/**
		})) {
			ZCLOG(ERROR) >> conn->error() << "goThread Failed" << End;
		}
*/
	});
	
	});

	return RPCClient::Instance().setupClient(ClientConfig());
}

bool RPCSpec::
pushRemoteCall(const oarchiver_type& data, RPCServerConnection::ptr conn) {
	RCDataCell::ptr cell = RCDataCell::create();
	cell->server_conn = conn;

	data["key"] & cell->key;

	rpc_handler_type handler;
	if (!handlers().getRPCHandler(handler, cell->key)) {
		return false;
	}

	oarchiver_type params;
	data["params"] & params;
	ZC_ASSERT( data["id"] & cell->id );
	ZC_ASSERT( data["tid"] & cell->tid );

	ConcurrentState::ptr state =
			ConcurrentState::create(bind(&RPCSpec::handleRemoteCallResult, this, _1, cell));

	handler.setParams(cell->key, params, cell->ret_data);
	return Concurrent::Instance().add(handler, state);
}

bool RPCSpec::
pushRemoteCall(const oarchiver_type& data, RPCClientConnection::ptr conn) {
	return pushCallback(data);
}

void RPCSpec::
handleRemoteCallResult(RuntimeContext::ptr rc, RCDataCell::ptr cell) {
	ZC_ASSERT(cell);
	ZC_ASSERT(cell->server_conn);

	ZCLOG(NOTE) << "Response to " << cell->id << End;

	iarchiver_type head, body;
	head["id"] & cell->id;

	if (rc && rc->error) {
		head["err"] & rc->error;

		ZC_DEBUG_ASSERT(false);
	}
	else {
		body[cell->key] & cell->ret_data;
	}

	iarchiver_type o;
	o["head"] & head;
	o["body"] & body;
	o["tid"] & cell->tid;

	if (!cell->server_conn->response(o.buffer())) {
		ZCLOG(ERROR) << "Handle Remote Call response failed, id=" << cell->id << End;
	}
}

bool RPCSpec::
makeAsyncCall(RPCClientConnection::ptr conn, const key_type& key, iarchiver_type params,
		const rpc_callback_type& handler, ConcurrentState::ptr state, const tick_type& timeout_ms) {
	iarchiver_type data;

	u32 msg_id = handlers().registerCallbackHandler(key, handler, state, conn->getRemoteConfig(), timeout_ms);

	ZCLOG(NOTE) << "Request Msg id=" << msg_id << End;

	data["id"] & msg_id;
	data["tid"] & Thread::getCurrentTid();
	data["key"] & key;
	data["params"] & params;

	if (conn->sendRequest(data.buffer())) {
		if (state)
			state->listen();

		return true;
	}

	handlers().removeCallbackHandler(msg_id);
	return false;
}

bool RPCSpec::
makeCall(RPCClientConnection::ptr server_conn, const key_type& key,
		iarchiver_type params, oarchiver_type& oar, const tick_type& timeout_ms) {
	iarchiver_type data;

	data["id"] & 1;
	data["tid"] & Thread::getCurrentTid();
	data["key"] & key;
	data["params"] & params;

	SharedBuffer receive;
	if (!server_conn->sendRequest(data.buffer(), receive))
		return false;

	return oar.buffer(receive);
}

bool RPCSpec::
makeNotify(RPCClientConnection::ptr conn, const key_type& key,
		iarchiver_type params) {
	iarchiver_type data;
	data[key] & params;

	return conn->sendRequest(data.buffer());
}

bool RPCSpec::
pushDataSync(const oarchiver_type& data) {
	map<serialization::key_type, oarchiver_type> o_map;
	data & o_map;

	bool ret = false;
	for (auto it = o_map.begin(); it != o_map.end(); ++it) {
//		RCDataCell::ptr cell = RCDataCell::create();
//		cell->key = it->first;

		data_sync_handler_type handler;
		if (!handlers().getDataSyncHandler(handler, it->first)) {
			ZCLOG(ERROR) << "SynData Handler Not Found For Key:" << it->first << End;
			continue;
		}

		handler.setParams(it->first, it->second);
		handler.push();
		/**
		ConcurrentState::ptr state =
				ConcurrentState::create(bind(&RPCManager::handleDataSynResult, this, _1, cell));
		Concurrent::Instance().add(handler, state);
		*/

		ret = true;
	}

	return ret;
}

bool RPCSpec::
pushCallback(const oarchiver_type& data) {
	oarchiver_type head;
	data["head"] & head;

	oarchiver_type body;
	data["body"] & body;

	u32	msg_id = 0;
	head["id"] & msg_id;

	ZCLOG(NOTE) << "msg_id=" << msg_id << End;

	if (msg_id > 0) {
		rpc_callback_type handler;
		ConcurrentState::ptr state;
		key_type key;
		if (handlers().popCallbackHandler(handler, state, key, msg_id)) {
			/**
			 * handler could be null
			 */
			if (handler) {
				oarchiver_type ret_data;
				Error error;

				head["err"] & error;
				body[key] & ret_data;

				handler.setParams(key, error, ret_data);
				handler.push();
			}

			if (state) {
				state->complete();
			}
		}
		else {
			ZCLOG(ERROR) << "handler not found for msg_id=" << msg_id << End;
		}
	}

	return true;
}

}}}
