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

namespace zertcore { namespace suit { namespace cmd {

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

namespace zertcore {

/**
 * Command:
 */
class TestCommand : public suit::cmd::CommandBase<session_type>
{
public:
	virtual ~TestCommand() {}

public:
	/**
	 * running in the main thread
	 */
	virtual void run(session_ptr session, const string& cmd, const rpc::oarchiver_type& params) {
		ZCLOG(NOTE) << "Run!" << End;
	}
};

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
		ZC_ASSERT( session_manager::Instance().setup() );
		cmd::reg<session_type>("key", new TestCommand);
	}

};

}

int main(int argc, char* argv[]) {
	zertcore::Game gate;

	gate.setup(argc, argv);
	gate.run();

	return 0;
}


