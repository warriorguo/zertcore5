/*
 * RPCManager.cpp
 *
 *  Created on: 2015年2月1日
 *      Author: Administrator
 */

#include <concurrent/Concurrent.h>
#include "RPCManager.h"

namespace zertcore { namespace concurrent { namespace rpc {

bool RPCManager::
registerHandler(const key_type& key, const rpc_handler_type& handler) {
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
putRemoteCall(const oachiver_type& data, RPCServerConnection::ptr conn) {
	RCDataCell::ptr cell = RCDataCell::create();
	cell->conn = conn;

	data["key"] & cell->key;

	rpc_handler_type orgin_handler;
	if (!getHandler(orgin_handler, cell->key)) {
		return false;
	}

	oachiver_type params;
	data["params"] & cell->params;

	data["id"] & cell->id;

	th_rpc_type handler(orgin_handler, th_rpc_type::params_type(cell->key, cell->params, cell->ret_data));
	ConcurrentState::ptr state =
			ConcurrentState::create(bind(&RPCManager::handleRemoteCallResult, this, _1, cell));

	return Concurrent::Instance().add(handler, state);
}

void RPCManager::
handleRemoteCallResult(const RunningContext& rc, RCDataCell::ptr cell) {
	ZC_ASSERT(cell);
	ZC_ASSERT(cell->conn);

	iachiver_type o;
	o["id"] & cell->id;

	if (rc.error) {
		o["err"] & rc.error;
	}
	else {
		o["ret"] & cell->ret_data;
	}

	cell->conn->write(o.buffer());
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

void RPCManager::
call(const key_type& key, const iachiver_type&) {
	;
}

void RPCManager::
call(const key_type& key, const iachiver_type&, const rpc_callback_type& handler) {
	;
}

}}}