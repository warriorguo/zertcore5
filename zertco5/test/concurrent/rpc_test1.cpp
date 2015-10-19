/*
 * rpc_test1.cpp
 *
 *  Created on: 2015年3月4日
 *      Author: Administrator
 */

#include <concurrent/rpc/RPCManager.h>

using namespace zertcore::concurrent::rpc;

void hello(const key_type&, const oachiver_type& params, iachiver_type& ret_data) {
	ret_data["ret"] & 1;
}

int main() {
	RPCManager::Instance().registerRPCHandler("hello", hello);
	return 0;
}


