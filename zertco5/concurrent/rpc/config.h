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
#include <net/config.h>

#include <serialize/Serializer.h>
#include <serialize/Unserializer.h>

#include <utils/msgpack/MsgPackStream.h>

namespace zertcore { namespace concurrent { namespace rpc {
using namespace zertcore::utils;

/**
 * use msgpack as RPC serializer & unserializer as default
 */
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

typedef serialization::Serializer<istream_type>
											iachiver_type;
typedef serialization::Unserializer<ostream_type>
											oachiver_type;


typedef ThreadHandler<void (const key_type&,
		const oachiver_type& params, iachiver_type& ret_data),
		Params<key_type, const oachiver_type&, iachiver_type&> >
											th_rpc_type;
typedef th_rpc_type::function_type			rpc_handler_type;


typedef ThreadHandler<void (const key_type&, const oachiver_type& ret_data),
		Params<key_type, const oachiver_type&> >
											th_rpc_callback_type;
typedef th_rpc_callback_type::function_type	rpc_callback_type;


typedef ThreadHandler<void (const key_type&, const oachiver_type& params),
		Params<key_type, const oachiver_type&> >
											th_data_sync_handler_type;
typedef	th_data_sync_handler_type::function_type
											data_sync_handler_type;


typedef ThreadHandler<void (const key_type&, iachiver_type&),
		Params<key_type, iachiver_type&> >	th_data_gen_type;
typedef	th_data_gen_type::function_type		data_gen_handler_type;

/**
 *
 */
const static u32 RPC_MAX_PACKAGE_SIZE		= 48 * 1024 * 1024;
}}}

namespace zertcore { namespace concurrent { namespace rpc {

/**
 * RPCRouterConfig
 */
struct RPCRouterConfig : public RemoteConfig {};

/**
 * RPCServerConfig
 */
struct RPCServerConfig :
		Serializable<RPCServerConfig>,
		Unserializable<RPCServerConfig>
{
	RemoteConfig				rc;

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
		archiver & rc;
		archiver["call_keys"] & call_keys;
		archiver["data_sync_keys"] & data_sync_keys;
	}

	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		return
			(archiver & rc) &&
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
	RemoteConfig				rc;

	/**
	 * binds data sync key
	 */
	set<string>					keys;

	template <class Archiver>
	void serialize(Archiver& archiver) const {
		archiver & rc;
		archiver["keys"] & keys;
	}

	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		return
			(archiver & rc) &&
			(archiver["keys"] & keys);
	}
};

}}}

#endif /* CONFIG_H_ */
