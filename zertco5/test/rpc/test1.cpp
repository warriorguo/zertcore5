/*
 * test1.cpp
 *
 *  Created on: 2015年2月3日
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
		RemoteConfig("127.0.0.1", 20010),
		RemoteConfig("127.0.0.1", 20011)
	));
	config.concurrent.thread_nums = 3;

	RT.init([&]() {
		RPCRouterConfig router_config;
		router_config.host = "127.0.0.1";
		router_config.port = 20007;

		RPCManager::Instance().connectRouter(router_config);

		RPCManager::Instance().registerRPCHandler("echo", [] (key_type,
				oarchiver_type params, iarchiver_type& ret_data) {
			string text;

			sleep(1000);
			params["text"] & text;
			ret_data["text"] & text;
		});

		RPCManager::Instance().setRPCReadyHandler([] () {
			ZCLOG(NOTE) << "RPC is now ready." << End;

			RPCManager::Instance().setRPCReadyHandler();

			iarchiver_type i;
			i["text"] & "show me the money";

			ZC_ASSERT(
			RPCManager::Instance().asyncCall("echo", i, [] (key_type key, Error error, oarchiver_type o) {
				string text;
				o["text"] & text;

				if (error) {
					ZCLOG(NOTE) << "Got Error:" << error << End;
				}

				ZCLOG(NOTE) << key << " ret=" << text << End;

			}) );
		});
		RPCManager::Instance().finishRegister();
	});

	RT.run();
	return 0;
}
