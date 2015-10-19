/*
 * test2.cpp
 *
 *  Created on: 2015年4月10日
 *      Author: Administrator
 */

#include <concurrent/rpc/RPCManager.h>
#include <thread/Thread.h>
#include <core/Runtime.h>
#include <config/Config.h>

using namespace zertcore;
using namespace zertcore::concurrent::rpc;

int main() {
	ZC_ASSERT(
	RPCManager::Instance().init(
		RemoteConfig("127.0.0.1", 20020),
		RemoteConfig("127.0.0.1", 20021)
	));
	config.concurrent.thread_nums = 3;

	RT.init([&]() {
		RPCRouterConfig router_config;
		router_config.host = "127.0.0.1";
		router_config.port = 20007;

		RPCManager::Instance().connectRouter(router_config);

		RPCManager::Instance().setRPCReadyHandler([] () {
			ZCLOG(NOTE) << "RPC is now ready." << End;

			for (int index = 0; index < 10000; ++index) {
				iarchiver_type i;
				i["text"] & ("operation cwal:" + lexical_cast<string>(index));

				ZC_ASSERT(
				RPCManager::Instance().asyncCall("echo", i, [] (key_type key, Error error, oarchiver_type o) {
					string text;
					o["text"] & text;

					ZCLOG(NOTE) << "echo ret=" << text.c_str() << End;

					RT.stop();
				}) );
			}

		});

		RPCManager::Instance().finishRegister();
	});

	RT.run();
	return 0;
}



