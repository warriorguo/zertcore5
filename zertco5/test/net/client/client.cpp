#include <net/PersistConnection.h>
#include <net/client/ClientBase.h>
#include <utils/Singleton.h>
#include <core/Runtime.h>

namespace zertcore {
using namespace zertcore::net;
using namespace zertcore::net::client;

class RPCClient;

class RPCClientConnection :
		public ConnectionBase<RPCClientConnection, RPCClient>
{
public:
	RPCClientConnection(RPCClient& service) : ConnectionBase<RPCClientConnection, RPCClient>(service) {}
	virtual ~RPCClientConnection() {}

public:
	virtual size_t onRead(const SharedBuffer& buffer) {
		::printf("onRead : %s\n\n\n", buffer.data());
		return buffer.size();
	}

public:
	void sendRequest() {
		string req("GET / HTTP/1.0\r\nHost: www.baidu.com\r\nConnection: close\r\n\r\n");
		asyncWrite((const u8 *)req.data(), req.size());
	}
};

class RPCClient :
		public ClientBase<RPCClient, RPCClientConnection>,
		public Singleton<RPCClient>
{
public:
	RPCClient() : ClientBase<RPCClient, RPCClientConnection>() {}
	virtual ~RPCClient() {}
};

}

int main() {
	zertcore::net::ClientConfig cc;
	cc.thread_nums = 1;
	cc.enable_ssl = false;

	zertcore::RPCClientConnection::ptr conn;
	zertcore::RPCClient::Instance().init(cc);

	conn = zertcore::RPCClient::Instance().connect("www.baidu.com", 80);
	ZC_ASSERT(conn);
	conn->sendRequest();

	RT.init([&conn] {

	});
//	zertcore::RPCClient::Instance().getIOService().run();
	RT.run();

	return 0;
}
