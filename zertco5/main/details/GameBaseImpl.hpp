/*
 * GameImpl.hpp
 *
 *  Created on: 2015年6月21日
 *      Author: Administrator
 */

#ifndef ZERTCORE_MAIN_DETAILS_GAMEBASEIMPL_HPP_
#define ZERTCORE_MAIN_DETAILS_GAMEBASEIMPL_HPP_

#include "../GameBase.h"

namespace zertcore {

typedef Serializer<BSONIStream>				iarchiver_type;
typedef Unserializer<BSONOStream>			oarchiver_type;

}

namespace zertcore {

template <class F, class C, class S>
void GameBase<F, C, S>::
init() {

}

template <class F, class C, class S>
typename GameBase<F, C, S>::config_type& GameBase<F, C, S>::
getConfig() {
	return config_;
}

template <class F, class C, class S>
void GameBase<F, C, S>::
setup(int argc, char* argv[]) {
	if (argc < 2) {
		::printf("Usage:\n   %s config_file\n\n", argv[0]);
		return;
	}

	SharedBuffer buffer;
	const char* filename(argv[1]);

	if (!utils::FileManager::getContent(filename, buffer)) {
		::printf("Get config file[%s] failed\n", filename);
		return;
	}

	oarchiver_type o;
	if (!o.buffer(buffer)) {
		::printf("Parse config data failed\n");
		return;
	}

	o & config_;

	for (auto it = config_.db_config.begin(); it != config_.db_config.end(); ++it) {
		ZC_ASSERT( mongodb::DatabaseManager::Instance().add(*it) );
	}

	if (config_.thread_amount)
		config.concurrent.thread_nums = config_.thread_amount;
	else
		config.concurrent.thread_nums = System::CPUCoreCount();

	if (config_.router_config) {
		ZC_ASSERT( rpc::RPCManager::setup(
			config_.rpc_server,
			config_.rpc_datasync
		) );

		rpc::RPCRouterConfig router_config;
		router_config.host = config_.router_config.host;
		router_config.port = config_.router_config.port;

		ZC_ASSERT( RT.setupRouter(router_config) );

		if (config_.enable_sync_tick) {
			ZC_ASSERT( update::UpdateClient::Instance().globalInit() );
			update::UpdateClient::Instance().registerHandler(bind(&GameBase<F, C, S>::onTick, this, _1));
		}

	}

	if (!server_.setup(config_.server_config)) {
		::printf("Server setup failed\n");
		return;
	}

	inited_ = true;
	onSetup();
}

template <class F, class C, class S>
void GameBase<F, C, S>::
run() {
	if (!inited_) {
		::printf("Run without init\n");
		return ;
	}

	RT.globalInit([this]() {
		globalInit();
	});

	RT.run();
}

template <class F, class C, class S>
void GameBase<F, C, S>::
globalInit() {
	this->template threadCheck();

	ZC_ASSERT( rpc::registerDataSync(rpc::cmd::CONFIG, config_) );

	/**
	 * on RPC Ready
	 */
	RPC.setRPCReadyHandler([this] () {
		onRPCReady();
	});

	onInit();
}

}


#endif /* MAIN_DETAILS_GAMEBASEIMPL_HPP_ */
