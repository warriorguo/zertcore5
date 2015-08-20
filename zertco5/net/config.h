/*
 * config.h
 *
 *  Created on: 2015年6月14日
 *      Author: Administrator
 */

#ifndef ZERTCORE_NET_CONFIG_H_
#define ZERTCORE_NET_CONFIG_H_

#include <pch.h>
#include <utils/types.h>

namespace zertcore {

/**
 * RemoteConfig
 */
struct RemoteConfig :
		Serializable<RemoteConfig>,
		Unserializable<RemoteConfig>
{
	string						host;
	u32							port;

	RemoteConfig() : port(0) {}
	RemoteConfig(const string& h, const u32& p):
		host(h), port(p) {}

	operator bool() const {
		return !host.empty() && port;
	}

	bool operator== (const RemoteConfig& rc) const {
		return port == rc.port && host == rc.host;
	}
	bool operator!= (const RemoteConfig& rc) const {
		return !(*this==(rc));
	}

	string toString() const {
		return host + ":" + lexical_cast<string>(port);
	}

	template <class Archiver>
	void serialize(Archiver& archiver) const {
		archiver["host"] & host;
		archiver["port"] & port;
	}

	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		return
			(archiver["host"] & host) &&
			(archiver["port"] & port);
	}
};

}



#endif /* NET_CONFIG_H_ */
