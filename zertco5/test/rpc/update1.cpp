/*
 * update1.cpp
 *
 *  Created on: 2015年5月28日
 *      Author: Administrator
 */
#include <concurrent/rpc/RPC.h>
#include <concurrent/update/UpdateClient.h>

#include <zertcore.h>

using namespace zertcore;
using namespace zertcore::concurrent::rpc;
using namespace zertcore::concurrent::update;

int main(int argc, char *argv[]) {
	u32 port = lexical_cast<u32>(argv[1]);
	config.concurrent.thread_nums = 3;

	ZC_ASSERT(
	RPC.setup(
		RemoteConfig("127.0.0.1", port),
		RemoteConfig("127.0.0.1", port + 1)
	));

	RPCRouterConfig router_config;
	router_config.host = "127.0.0.1";
	router_config.port = 20007;

	RT.globalInit([&]() {
		ZC_ASSERT(RT.setupRouter(router_config));
		ZC_ASSERT(UpdateClient::Instance().globalInit());

		UpdateClient::Instance().registerHandler([] (const tick_type& tick) {
			ZCLOG(NOTICE) << "Got tick!" << End;
		});
	});
	RT.run();

	return 0;
}
