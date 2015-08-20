/*
 * Game.h
 *
 *  Created on: 2015年6月20日
 *      Author: Administrator
 */

#ifndef ZERTCORE_MAIN_GAMEBASE_H_
#define ZERTCORE_MAIN_GAMEBASE_H_

#include <zertcore.h>

namespace zertcore {
using namespace zertcore::utils;
using namespace zertcore::concurrent;
using namespace zertcore::db;
using namespace zertcore::net;
}

namespace zertcore {

/**
 * ConfigBase
 */
struct ConfigBase :
		public SerializableObject
{
	ZCVal(vector<mongodb::MongoDBAdapterConfig>,
								db_config);
	ZCVal(RemoteConfig,			rpc_server);
	ZCVal(RemoteConfig,			rpc_datasync);
	ZCVal(u32,					enable_sync_tick);
	ZCVal(RemoteConfig,			router_config);
	ZCVal(ServerConfig,			server_config);
	ZCVal(u32,					thread_amount);
};

}

namespace zertcore {

namespace non{
/**
 * NonServer
 */
struct NonServer
{
	bool setup(const ServerConfig& config) { return true; }
};

}

/**
 * Game<Final, Config>
 */
template <class Final, class Config, class Server = non::NonServer>
class GameBase :
		public utils::Singleton<Final, utils::NoneChecker>
{
public:
	typedef Config							config_type;
	typedef Server							server_type;

public:
	virtual ~GameBase() {}

public:
	virtual void init() final;

public:
	void setup(int argc, char* argv[]);
	void run();

private:
	void globalInit();

public:
	/**
	 * after finish setup called
	 * Notice: the thread here was not launched!
	 */
	virtual void onSetup() {}
	/**
	 * after finish initial
	 * Notice: running in the main thread.
	 * 1) Register the RPC Handlers here
	 */
	virtual void onInit() {}
	/**
	 * running in the main thread
	 * Would call or notify the RPC here
	 */
	virtual void onRPCReady() {}
	virtual void onTick(u32 count) {}

protected:
	config_type& getConfig();

private:
	bool						inited_{false};
	config_type					config_;
	server_type					server_;
};

}

#endif
