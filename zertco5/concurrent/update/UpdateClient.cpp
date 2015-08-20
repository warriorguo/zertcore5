/*
 * UpdateClient.cpp
 *
 *  Created on: 2015年5月7日
 *      Author: Administrator
 */

#include "UpdateClient.h"
#include "../rpc/RPCManager.h"

#include <core/Runtime.h>
#include <details.h>

namespace zertcore { namespace concurrent { namespace update {
using namespace zertcore::concurrent::rpc;
}}}

namespace zertcore { namespace concurrent { namespace update {

bool UpdateClient::
globalInit() {
	RT.initHandler([this] () {
		enabled_ = true;

		if(!RPCManager::Instance().registerRPCHandler(cmd::UPDATE,
				bind(&UpdateClient::cbUpdateOnce, this, _1, _2, _3)))
			ZCLOG(FATAL) << "Register Updater Client failed" << End;
	});

	return true;
}

void UpdateClient::
cbUpdateOnce(rpc::key_type key, rpc::oarchiver_type params,
			rpc::iarchiver_type& ret_data) {
	u32 count;
	ZC_ASSERT( params["count"] & count );

	for_each(updater_list_.begin(), updater_list_.end(), [&] (updater_type& handler) {
		handler.setParams(count);
		ZC_ASSERT( Concurrent::Instance().add(handler, concurrentState()) );
	});
}

UpdateClient::updater_key_type UpdateClient::
registerHandler(const updater_type& handler) {
	if (!enabled_) {
		ZCLOG(FATAL) << "Update Client was not enabled." << End;
	}
	return updater_list_.insert(updater_list_.end(), handler);
}

void UpdateClient::
unregisterHandler(updater_key_type key) {
	updater_list_.erase(key);
}

}}}

