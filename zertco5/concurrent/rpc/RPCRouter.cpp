/*
 * RPCRouter.cpp
 *
 *  Created on: 2015年3月3日
 *      Author: Administrator
 */

#include <core/Runtime.h>

#include "config.h"
#include "RPCRouter.h"
#include "RPCSpec.h"

#include "RPCClient.h"

#include <details.h>

namespace zertcore { namespace concurrent { namespace rpc {

RPCRouter::RPCRouter() : id_counter_(0) {
	;
}

bool RPCRouter::
globalInit() {
	RT.initHandler([this] () {
		ZC_ASSERT( RPCSpec::registerRPCHandler(cmd::REG_RPC_SYNC_SERVER,
				bind(&RPCRouter::SyncServer, this, _1, _2, _3)) );

		ZC_ASSERT( RPCSpec::registerRPCHandler(cmd::REG_RPC_SYNC_CLIENT,
				bind(&RPCRouter::SyncClient, this, _1, _2, _3)) );

		RPCClientConnection::setShutdownHandler(bind(&RPCRouter::onConnectionClose, this, _1));
	});

	return true;
}

void RPCRouter::
onConnectionClose(RPCClientConnection::ptr conn) {
	string rc_key(conn->getRemoteConfig().toString());

	conn_map_.erase(rc_key);
	svr_info_map_.erase(rc_key);

	SyncData();
}

void RPCRouter::
SyncServer(key_type key, oarchiver_type params, iarchiver_type& ret_data) {
	RPCServerConfig config;
	bool close_flag = false;

	if (!(params & config)) {
		ZCLOG(WARNING) << "SyncServer() : Empty Info" << End;
		return ;
	}

	params["close"] & close_flag;

	if (close_flag) {
		svr_info_map_.erase(config.rc.toString());
	}
	else {
		/**
		 * Check ID
		 */
		if (config.id > 0) {
			for (auto it = svr_info_map_.begin(); it != svr_info_map_.end(); ++it) {
				if (config.id == it->second.id && config.rc != it->second.rc) {
					ZCLOG(ERROR) << "Id duplicated with " << it->second.rc.toString() << End;
					return ;
				}
			}
		}
		else {
			config.id = ++id_counter_;
		}

		ret_data["id"] & config.id;
		svr_info_map_[config.rc.toString()] = config;
	}

	SyncData();
}

void RPCRouter::
SyncClient(key_type key, oarchiver_type params, iarchiver_type& ret_data) {
	RPCClientConfig config;
	bool close_flag = false;

	if (!(params & config)) {
		ZCLOG(WARNING) << "SyncClient() : Empty Info" << End;
		return ;
	}

	params["close"] & close_flag;
	string rc_key(config.rc.toString());

	if (close_flag) {
		cli_info_map_.erase(rc_key);

		/**
		 * erase the client connection at the same time
		 */
		conn_map_.erase(rc_key);
	}
	else {
		/**
		 * make the connection to the client at the same time
		 */
		RPCClientConnection::ptr conn = RPCClient::Instance().connect(config.rc);
		if (!conn) {
			ZCLOG(ERROR) << "Failed to connect to " << config.rc.toString() << End;
			return ;
		}

		conn->enableHeartbeat(TIMEOUT);

		conn_map_[rc_key] = conn;
		cli_info_map_[rc_key] = config;
	}

	SyncData();
}

void RPCRouter::
SyncData() {
	if (conn_map_.empty()) {
		ZCLOG(NOTE) << "Connection map was empty" << End;
		return ;
	}

	iarchiver_type svr_i;
	svr_i & svr_info_map_;

	iarchiver_type cli_i;
	cli_i & cli_info_map_;

	for (auto it = conn_map_.begin(); it != conn_map_.end(); ++it) {
		RPCSpec::Instance().makeNotify(it->second, cmd::REG_RPC_SYNC_SERVER, svr_i);
		RPCSpec::Instance().makeNotify(it->second, cmd::REG_RPC_SYNC_CLIENT, cli_i);
	}
}

}}}


