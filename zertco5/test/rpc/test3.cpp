/*
 * test3.cpp
 *
 *  Created on: 2015年4月22日
 *      Author: Administrator
 */

#include <concurrent/rpc/RPCManager.h>
#include <thread/Thread.h>
#include <core/Runtime.h>
#include <config/Config.h>

using namespace zertcore;
using namespace zertcore::concurrent::rpc;

static int global_index = 1;

void __cb(key_type key, Error error, oarchiver_type o) {
	iarchiver_type i;
	i["text"] & ("operation cwal:" + lexical_cast<string>(++global_index));

	string text;
	o["text"] & text;

	if (error) {
		ZCLOG(NOTE) << "Got error:" << error << End;
		return ;
	}

	ZCLOG(NOTE) << "echo ret=" << text.c_str() << End;

	for (int j = 0; j < global_index; ++j)
	ZC_ASSERT(
	RPCManager::Instance().asyncCall("echo", i, __cb)
	);
}

int main(int argc, char *argv[]) {
	u32 port = 0;
	if (argc != 2) {
		return 1;
	}

	port = lexical_cast<u32>(argv[1]);

	ZC_ASSERT(
	RPCManager::Instance().init(
		RemoteConfig("127.0.0.1", port),
		RemoteConfig("127.0.0.1", port + 1)
	));
	config.concurrent.thread_nums = 0;

	RT.init([&]() {
		RPCRouterConfig router_config;
		router_config.host = "127.0.0.1";
		router_config.port = 20007;

		RPCManager::Instance().connectRouter(router_config);

		RPCManager::Instance().setRPCReadyHandler([] () {
			ZCLOG(NOTE) << "RPC is now ready." << End;

			{
				iarchiver_type i;
				i["text"] & ("operation cwal:" + lexical_cast<string>(global_index));

				ZC_ASSERT(
				RPCManager::Instance().asyncCall("echo", i, __cb)
				);
			}

		});

		RPCManager::Instance().finishRegister();
	});

	RT.run();
	return 0;
}



