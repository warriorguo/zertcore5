/*
 * server_config.h
 *
 *  Created on: 2015年7月19日
 *      Author: Administrator
 */

#ifndef ZERTCORE_G_NET_SERVER_CONFIG_H_
#define ZERTCORE_G_NET_SERVER_CONFIG_H_

#include "config.h"

#ifndef ZC_SERVER_MAX_BACK_LOG
#  define ZC_SERVER_MAX_BACK_LOG			128
#endif

#ifndef ZC_SERVER_MAX_PEER_SIZE
#  define ZC_SERVER_MAX_PEER_SIZE			(1024 * 128)
#endif

namespace zertcore { namespace net {

struct ServiceConfig
{
	u32							thread_nums{1};

//	bool						enable_ssl{false};
//	SSLConfig					ssl_config;
};

/**
 * ServerConfig
 */
struct ServerConfig :
	Serializable<ServerConfig>,
	Unserializable<ServerConfig>,
	public ServiceConfig
{
	string						host;
	u32							port{0};
	u32							accept_nums{1};
	u32							back_log{ZC_SERVER_MAX_BACK_LOG};

	u32							peer_size{ZC_SERVER_MAX_PEER_SIZE};
	u32							channel_size{1};

	ServerConfig& operator= (const RemoteConfig& rc) {
		host = rc.host;
		port = rc.port;
		return *this;
	}

	template <class Archiver>
	void serialize(Archiver& archiver) const {
		archiver["host"] & host;
		archiver["port"] & port;

		archiver["thread_nums"] & thread_nums;
		archiver["accept_nums"] & accept_nums;
		archiver["back_log"] & back_log;
		archiver["peer_size"] & peer_size;
		archiver["channel_size"] & channel_size;
	}

	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		archiver["host"] & host;
		archiver["port"] & port;

		archiver["thread_nums"] & thread_nums;
		archiver["accept_nums"] & accept_nums;
		archiver["back_log"] & back_log;
		archiver["peer_size"] & peer_size;
		archiver["channel_size"] & channel_size;

		return !host.empty() && port;
	}
};

}}


#endif /* NET_SERVER_CONFIG_H_ */
