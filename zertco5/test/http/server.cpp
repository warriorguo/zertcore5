/*
 * server.cpp
 *
 *  Created on: 2015年10月19日
 *      Author: Administrator
 */

#include <zertcore.h>

using namespace zertcore;

int main() {
	net::HttpServer server;
	config.concurrent.thread_nums = 1;

	ZC_ASSERT( server.init() );

	utils::SharedBuffer sb;
	sb.assign("show me the money");

	RT.globalInit([&]() {
		server.setHandler([&](ServerConnection::ptr conn) {
			conn->response(sb);
		});
	});

	RT.run();
	return 0;
}

