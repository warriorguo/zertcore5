/*
 * config.h
 *
 *  Created on: 2015��1��13��
 *      Author: Administrator
 */

#ifndef ZERTCORE_NET_CONFIG_H_
#define ZERTCORE_NET_CONFIG_H_

#include <pch.h>
#include <utils/types.h>

#include <serialize/Serializer.h>
#include <serialize/Unserializer.h>

namespace zertcore {

/**
 * RemoteConfig
 */
struct RemoteConfig :
		serialization::Serializer<RemoteConfig>,
		serialization::Unserializer<RemoteConfig>
{
	string						host;
	u32							port;

	RemoteConfig(const string& h, const u32& p):
		host(h), port(p) {}

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

namespace zertcore { namespace net{

/**
 * SSLConfig
 *
openssl genrsa -des3 -out [private_key_file] 1024
openssl req -new -key [private_key_file] -out server_tmp.csr
openssl x509 -req -days 3650 -in server_tmp.csr -signkey [private_key_file] -out [certificate_chain_file]
openssl dhparam -out dh_file 512

for client:
ctx.load_verify_file(certificate_chain_file);
 */
struct SSLConfig
{
	string						certificate_chain_file;
	string						private_key_file;
	string						dh_file;

	string						password;
};

struct ServiceConfig
{
	u32							thread_nums{1};

	bool						enable_ssl{false};
	SSLConfig					ssl_config;
};

}}


#endif /* CONFIG_H_ */
