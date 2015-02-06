/*
 * RPCManager.cpp
 *
 *  Created on: 2015年2月1日
 *      Author: Administrator
 */

#include <concurrent/Concurrent.h>
#include "RPCManager.h"

namespace zertcore { namespace concurrent { namespace rpc {

RPCManager::
RPCManager() : msg_id_base_(0) {}

bool RPCManager::
registerHandler(const key_type& key, const rpc_handler_type& handler) {
	if (isEmpty(key))
		return false;

	rpc_handler_map_[key] = handler;
	return true;
}

bool RPCManager::
getHandler(rpc_handler_type& handler, const key_type& key) {
	rpc_handler_map_type::iterator it = rpc_handler_map_.find(key);
	if (it == rpc_handler_map_.end())
		return false;

	handler = it->second;
	return true;
}

bool RPCManager::
pushRemoteCall(const oachiver_type& data, RPCServerConnection::ptr conn) {
	RCDataCell::ptr cell = RCDataCell::create();
	cell->server_conn = conn;

	data["key"] & cell->key;

	rpc_handler_type orgin_handler;
	if (!getHandler(orgin_handler, cell->key)) {
		return false;
	}

	oachiver_type params;
	data["params"] & cell->data;

	data["id"] & cell->id;

	th_rpc_type handler(orgin_handler, {cell->key, cell->data, cell->ret_data});
	ConcurrentState::ptr state =
			ConcurrentState::create(bind(&RPCManager::handleRemoteCallResult, this, _1, cell));

	return Concurrent::Instance().add(handler, state);
}

void RPCManager::
handleRemoteCallResult(const RunningContext& rc, RCDataCell::ptr cell) {
	ZC_ASSERT(cell);
	ZC_ASSERT(cell->server_conn);

	iachiver_type head, body;
	head["id"] & cell->id;

	if (rc.error) {
		head["err"] & rc.error;
	}
	else {
		body[cell->key] & cell->ret_data;
	}

	iachiver_type o;
	o["head"] & head;
	o["body"] & body;

	cell->server_conn->response(o.buffer());
}

bool RPCManager::
registerDataSyncHandler(const key_type& key, const data_sync_handler_type& handler) {
	data_sync_handler_map_[key] = handler;
	return false;
}

bool RPCManager::
getDataSyncHandler(data_sync_handler_type& handler, const key_type& key) {
	data_sync_handler_map_type::iterator it = data_sync_handler_map_.find(key);
	if (it == data_sync_handler_map_.end())
		return false;

	handler = it->second;
	return true;
}

bool RPCManager::
pushDataSynHandler(const oachiver_type& data, RPCClientConnection::ptr conn) {
	map<oachiver_type::key_type, oachiver_type> o_map;
	data & o_map;

	bool ret = false;
	for (auto it = o_map.begin(); it != o_map.end(); ++it) {
		RCDataCell::ptr cell = RCDataCell::create();
		cell->client_conn = conn;
		cell->key = it->first;
		cell->data = it->second;

		data_sync_handler_type orgin_handler;
		if (!getDataSyncHandler(orgin_handler, cell->key)) {
			ZCLOG(ERROR) << "SynData Handler Not Found For Key:" << cell->key << End;
			continue;
		}

		th_data_sync_handler_type handler(orgin_handler, {cell->key, cell->data});
		ConcurrentState::ptr state =
				ConcurrentState::create(bind(&RPCManager::handleDataSynResult, this, _1, cell));
		Concurrent::Instance().add(handler, state);

		ret = true;
	}

	return ret;
}

void RPCManager::
handleDataSynResult(const RunningContext& rc, RCDataCell::ptr cell) {
	;
}

void RPCManager::
notify(const key_type& key, const iachiver_type& params) {
	iachiver_type data;

	data["params"] & params;

	// TODO: Get RPCServer
	client_.sendRequest(data.buffer());
}

void RPCManager::
call(const key_type& key, const iachiver_type& params, const rpc_callback_type& handler) {
	iachiver_type data;
	registerCallbackHandler(handler, data);

	data["params"] & params;

	// TODO: Get RPCServer base by key
	client_.sendRequest(data.buffer());
}

void RPCManager::
registerCallbackHandler(const rpc_callback_type& handler, iachiver_type& data) {
	rpc_callback_map_.insert(
			rpc_callback_map_type::value_type(++msg_id_base_, handler));
	data["id"] & msg_id_base_;
}

}}}
