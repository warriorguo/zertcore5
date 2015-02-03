/*
 * config.h
 *
 *  Created on: 2015年1月27日
 *      Author: Administrator
 */

#ifndef ZERTCORE_RPC_CONFIG_H_
#define ZERTCORE_RPC_CONFIG_H_

#include <pch.h>
#include <utils/types.h>

#include <thread/ThreadHandler.h>

#include <serialize/Serializer.h>
#include <serialize/Unserializer.h>

#include <utils/msgpack/MsgPackStream.h>

namespace zertcore { namespace concurrent { namespace rpc {
using namespace zertcore::utils;

#ifndef ZC_RPC_ISTREAM_TYPE
typedef messagepack::MsgPackIStream			istream_type;
#else
typedef ZC_RPC_ISTREAM_TYPE					istream_type;
#endif

#ifndef ZC_RPC_OSTREAM_TYPE
typedef messagepack::MsgPackOStream			ostream_type;
#else
typedef ZC_RPC_OSTREAM_TYPE					ostream_type;
#endif

#ifndef ZC_RPC_KEY_TYPE
typedef string								key_type;
#else
typedef ZC_RPC_KEY_TYPE						key_type;
#endif

/**
 * use msgpack as RPC serializer & unserializer as default
 */
typedef serialization::Serializer<istream_type>
											iachiver_type;
typedef serialization::Unserializer<ostream_type>
											oachiver_type;

typedef ThreadHandler<void (const key_type&,
		const oachiver_type& params, iachiver_type& ret_data),
		key_type, const oachiver_type&, iachiver_type&>
											th_rpc_type;
typedef th_rpc_type::function_type			rpc_handler_type;

typedef ThreadHandler<void (const key_type&, const oachiver_type& ret_data)>
											th_rpc_callback_type;
typedef th_rpc_callback_type::function_type	rpc_callback_type;

typedef ThreadHandler<void (const key_type&, const oachiver_type& params)>
											th_data_sync_type;
typedef	th_data_sync_type::function_type	data_sync_handler_type;

/**
 *
 */
const static u32 RPC_MAX_PACKAGE_SIZE		= 48 * 1024 * 1024;
}}}

namespace zertcore { namespace concurrent { namespace rpc {

/**
 * RPCRouterConfig
 */
struct RPCRouterConfig :
		Serializable<RPCRouterConfig>,
		Unserializable<RPCRouterConfig>
{
	string						host;
	u32							port;

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

/**
 * RPCServerConfig
 */
struct RPCServerConfig :
		Serializable<RPCServerConfig>,
		Unserializable<RPCServerConfig>
{
	string						host;
	u32							port;

	/**
	 * binds remote call list
	 */
	set<string>					call_keys;
	/**
	 * ???
	 * would sync data keys list
	 */
	set<string>					data_sync_keys;

	template <class Archiver>
	void serialize(Archiver& archiver) const {
		archiver["host"] & host;
		archiver["port"] & port;
		archiver["call_keys"] & call_keys;
		archiver["data_sync_keys"] & data_sync_keys;
	}

	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		return
			(archiver["host"] & host) &&
			(archiver["port"] & port) &&
			(archiver["call_keys"] & call_keys) &&
			(archiver["data_sync_keys"] & data_sync_keys);
	}
};

/**
 * RPCClientConfig
 */
struct RPCClientConfig :
		Serializable<RPCClientConfig>,
		Unserializable<RPCClientConfig>
{
	string						host;
	u32							port;

	/**
	 * binds data sync key
	 */
	set<string>					keys;

	template <class Archiver>
	void serialize(Archiver& archiver) const {
		archiver["host"] & host;
		archiver["port"] & port;
		archiver["keys"] & keys;
	}

	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		return
			(archiver["host"] & host) &&
			(archiver["port"] & port) &&
			(archiver["keys"] & keys);
	}
};

}}}

#endif /* CONFIG_H_ */
