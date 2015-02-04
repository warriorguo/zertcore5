/*
 * test1.cpp
 *
 *  Created on: 2015年2月3日
 *      Author: Administrator
 */

#include <concurrent/rpc/RPCManager.h>
#include <thread/Thread.h>

using namespace zertcore;
using namespace zertcore::concurrent;
using namespace zertcore::concurrent::rpc;

int main() {
	RPCManager::Instance().registerHandler("echo", [] (const key_type&,
			const oachiver_type& params, iachiver_type& ret_data) {
		string text;
		params["text"] & text;
		ret_data["text"] & text;
	});

	RPCManager::Instance().registerHandler("error", [] (const key_type&,
			const oachiver_type& params, iachiver_type& ret_data) {
		Thread::getCurrentRunningContext().error.Error("Hello Error");
	});

	return 0;
}
