/*
 * gate_server.cpp
 *
 *  Created on: 2015年7月23日
 *      Author: Administrator
 */

#include <zertcore.h>

namespace zertcore { namespace net {
class GateServer;
class GateConnection;
}}

/**
namespace zertcore { namespace suit { namespace cmd {

bool parseCommand(const SharedBuffer& sb, key_type& key, params_type& params) {
	params_type oar;
	oar["k"] & key;
	oar["p"] & params;

	return !key.empty();
}

}}}
*/

namespace zertcore { namespace suit {

struct GateSessionData : Serializable<GateSessionData>
{
	u32							target;

	template <class Archiver>
	void serialize(Archiver& archiver) const {
		archiver["target"] & target;
	}
};

typedef Session<32, GateSessionData, GateConnection>
											session_type;
typedef SessionManager<32, GateSessionData, GateConnection>
											session_manager;
}}

namespace zertcore { namespace net {
using namespace zertcore::suit;

/**
 * GateConnection
 */
class GateConnection : public udp::PackageConnection<GateConnection, GateServer>
{
public:
	explicit GateConnection(GateServer& server, udp::peer_ptr peer) :
		udp::PackageConnection<GateConnection, GateServer>(server, peer) {}

	ZC_TO_STRING("Connection" << getRemoteConfig().toString());

public:
	void setSession(session_type::ptr session) {session_ = session;}
	session_type::ptr getSession() {return session_;}

private:
	session_type::ptr			session_;
};

/**
 * GateServer
 */
class GateServer : public udp::ServerBase<GateServer, GateConnection>
{
};

}}

namespace zertcore {

/**
 * GateConfig
 */
struct GateConfig : public ConfigBase
{};


/**
 * Gate
 */
class Gate : public GameBase<Gate, GateConfig, net::GateServer>
{
public:
	virtual void onInit() {
		session_manager::Instance().setupSync();
		/**

		*/

		GateConnection::setPackageHandler([&] (GateConnection::ptr conn, SharedBuffer sb) {
			session_manager::go([conn, sb] {
				bool is_admin = false; //check whether conn->getRemoteConfig() was in admin IP list;
				ZCLOG(NOTE) << "Get message with size=" << sb.size() << End;

				session_type::ptr session = conn->getSession();
				if (!session) {
					session = session_manager::Instance().createSession();
					conn->setSession(session);

					ZC_ASSERT( session->setMaster() );
					session->setGate();

					if (is_admin)
						session->setPriority(PRIORITY_HIGH);
				}

				if (sb.size() > 10)
					session->data().target = 1;
				else
					session->data().target = 2;

				if (!session->pushMessage(sb, conn)) {
					ZCLOG(ERROR) >> conn->error() << "Session push message failed" << End;
				}
			});

		});
	}

};

}

int main(int argc, char* argv[]) {
	zertcore::Gate gate;

	gate.setup(argc, argv);
	gate.run();

	return 0;
}
