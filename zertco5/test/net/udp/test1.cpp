/*
 * test1.cpp
 *
 *  Created on: 2015年6月14日
 *      Author: Administrator
 */

#include <zertcore.h>

namespace zertcore { namespace net { namespace udp {

class Server;

class Connection : public PackageConnection<Connection, Server>
{
public:
	explicit Connection(Server& server, peer_ptr peer) :
		PackageConnection<Connection, Server>(server, peer) {}

	ZC_TO_STRING("Connection" << getRemoteConfig().toString());
};

class Server : public ServerBase<Server, Connection>
{
};

}}}

using namespace zertcore;

int main() {
	config.concurrent.thread_nums = 1;
	net::udp::Server server;
	net::udp::ServerConfig server_config;
	server_config.host = "0.0.0.0";
	server_config.port = 1333;

	ZC_ASSERT( server.setup(server_config) );

	RT.globalInit([&] () {

	});
	RT.run();

	return 0;
}
