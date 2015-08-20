/*
 * RPCHandlers.cpp
 *
 *  Created on: 2015-2-16
 *      Author: Administrator
 */
#include "RPCSpec.h"
#include "RPCRouterClient.h"

#include <log/Log.h>
#include <utils/perf/Performance.h>

#include <details.h>

namespace zertcore { namespace concurrent { namespace rpc {
using namespace zertcore::utils;

RPCHandlers::rpc_handler_map_type
								RPCHandlers::rpc_handler_map_;
RPCHandlers::data_sync_handler_map_type
								RPCHandlers::data_sync_handler_map_;
RPCRouterClient					RPCHandlers::router_;
}}}

namespace zertcore { namespace concurrent { namespace rpc {

bool RPCHandlers::
init() {
	rpc_callback_map_.init(bind(&RPCHandlers::onTimeout, this, _1), RPCSpec::DEFAULT_CB_TIME_OUT);
	return true;
}

bool RPCHandlers::
isSystemKey(const key_type& key) {
	return key.empty() || key[0] == '*';
}

bool RPCHandlers::
getRPCHandler(rpc_handler_type& handler, const key_type& key) {
	auto it = rpc_handler_map_.find(key);
	if (it == rpc_handler_map_.end())
		return false;

	handler = it->second;
	return true;
}

bool RPCHandlers::
getDataSyncHandler(data_sync_handler_type& handler, const key_type& key) {
	auto it = data_sync_handler_map_.find(key);
	if (it == data_sync_handler_map_.end())
		return false;

	handler = it->second;
	return true;
}

bool RPCHandlers::
popCallbackHandler(rpc_callback_type& handler, ConcurrentState::ptr& state,
		key_type& key, const u64& id) {
	auto it = rpc_callback_map_.get(id);
	if (!it)
		return false;

	handler = it->handler;
	key = it->key;
	state = it->state;

	tick_type take_time = rpc_callback_map_.getExpiredTick(id);
	PerformanceManager::Instance().add("host:"+it->rc.toString(), take_time);

	rpc_callback_map_.remove(id);
	return true;
}

bool RPCHandlers::
removeCallbackHandler(const u64& id) {
	return rpc_callback_map_.remove(id);
}

bool RPCHandlers::
registerRPCHandler(const key_type& key, const rpc_handler_type& handler) {
	if (isEmpty(key))
		return false;

	rpc_handler_type& h = rpc_handler_map_[key] = handler;
	router_.registerRPCHandlers(key);

	if (isSystemKey(key)) {
		h.setPriority(PRIORITY_HIGH);
	}

	return true;
}

bool RPCHandlers::
registerDataSyncHandler(const key_type& key, const data_sync_handler_type& handler,
		const condition_expr_type& cond) {
	if (isEmpty(key))
		return false;

	data_sync_handler_type& h = data_sync_handler_map_[key] = handler;
	router_.registerDataSyncHandlers(key, cond);

	if (isSystemKey(key)) {
		h.setPriority(PRIORITY_HIGH);
	}

	return true;
}

u32 RPCHandlers::
registerCallbackHandler(const key_type& key, const rpc_callback_type& handler,
		ConcurrentState::ptr state, const RemoteConfig& rc, const tick_type& timeout) {
	CallbackCell cell;
	cell.key = key;
	cell.handler = handler;
	cell.rc = rc;
	cell.state = state;

	if (isSystemKey(key)) {
		cell.handler.setPriority(PRIORITY_HIGH);
	}

	rpc_callback_map_.set(++msg_id_base_, cell, timeout);
	return msg_id_base_;
}

void RPCHandlers::
onTimeout(const u64& id) {
	/**
	 * the rpc_callback_map_ would automatically delete the recording.
	 */
	auto it = rpc_callback_map_.get(id);

	tick_type x = rpc_callback_map_.getExpiredTick(id);
	ZCLOG(ERROR) << "Timeout[ID:" << id << "] expired=" << x << End;

	ZC_DEBUG_ASSERT(it);

	PerformanceManager::Instance().add("host:"+it->rc.toString(), time_type(10));

	/**
	 * setup timeout error for the handler
	 */
	if (it->handler) {
		it->handler.setParams(it->key, error::TIME_OUT, oarchiver_type());
		it->handler.push();
	}

	if (it->state) {
		it->state->complete(error::TIME_OUT);
	}
}

bool RPCHandlers::
syncRouter(ConcurrentState::ptr state) {
	router_.notifyRouter(state);
	return true;
}

}}}


