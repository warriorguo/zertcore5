/*
 * rpc_router_main.cpp
 *
 *  Created on: 2015年3月5日
 *      Author: Administrator
 */

#include <zertcore.h>

#include <concurrent/update/UpdateServer.h>
#include <concurrent/rpc/RPCRouter.h>

using namespace zertcore;
using namespace zertcore::concurrent::rpc;
using namespace zertcore::concurrent::update;

int main() {
	UpdateConfig update_config;
	RPCRouterConfig router_config;

	RPCRouter router;

	update_config.interval = 1000;
	router_config.host = "127.0.0.1";
	router_config.port = 8080;

	config.concurrent.thread_nums = 1;

	ZC_ASSERT(RPCManager::setup(
		RemoteConfig("0.0.0.0", router_config.port),
		RemoteConfig("0.0.0.0", router_config.port + 10)
	));

	RT.setupRouter(router_config);
	RT.globalInit([&]() {
		router.globalInit();
		ZC_ASSERT( UpdateServer::Instance().globalInit(update_config) );

		/**
		 * RPC Ready
		 */
		RPCManager::Instance().setRPCReadyHandler([] () {
			ZC_ASSERT( UpdateServer::Instance().start() );
		});

	});
	RT.run();

	return 0;
}
