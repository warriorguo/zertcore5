/*
 * backgate_server.cpp
 *
 *  Created on: 2015年7月23日
 *      Author: Administrator
 */

#include <zertcore.h>

namespace zertcore {

struct SomeData {};

typedef suit::Session<128, SomeData>		session_type;
typedef session_type::ptr					session_ptr;
typedef suit::SessionManager<128, SomeData>	session_manager;
}
/**
namespace zertcore { namespace suit { namespace cmd {

CommandBase<session_type>::ptr fetchCommand<session_type>(const SharedBuffer& sb) {
	;
}

bool parseCommand(const SharedBuffer& sb, key_type& key, params_type& params) {
	params_type oar;
	if (!oar.buffer(sb)) {
		ZCLOG(ERROR) << "parse failed" << End;
		return false;
	}

	oar["k"] & key;
	oar["p"] & params;

	::printf("KEY:%s\n", key.c_str());

	return !key.empty();
}

}}}
*/
namespace zertcore {

/**
 * Command:

class TestCommand : public suit::cmd::CommandBase<session_type>
{
public:
	virtual ~TestCommand() {}

public:
	/**
	 * running in the main thread
	 *
	virtual void run(session_ptr session, const string& cmd, const rpc::oarchiver_type& params) {
//		ZCLOG(NOTE) << "Run!" << End;
		string echo;
		params["echo"] & echo;

		SharedBuffer sb;
		session->send(sb);
	}
};
*/
}

namespace zertcore {

/**
 * GateConfig
 */
struct GameConfig : public ConfigBase
{};


/**
 * Gate
 */
class Game : public GameBase<Game, GameConfig>
{
public:
	virtual void onSetup() {;}

	virtual void onInit() {
		session_manager::Instance().setup();
		session_manager::Instance().setupSync(rpc::condition("target", cond::EQU, 1));
		session_type::setDataHandler([] (session_type::ptr sess, const SharedBuffer& sb) {
			ZCLOG(NOTE) << "Got message with size =" << sb.size() << End;
			sess->send(sb);
		});
	}

};

}

int main(int argc, char* argv[]) {
	zertcore::Game gate;

	gate.setup(argc, argv);
	gate.run();

	return 0;
}


