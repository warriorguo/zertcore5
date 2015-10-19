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
#include <net/server_config.h>

#include <serialize/Serializer.h>
#include <serialize/Unserializer.h>

#include <utils/msgpack/MsgPackStream.h>
#include <utils/condition/Condition.h>
#include <utils/string/KeyString.h>

#include <db/mongodb/serialize/BSONStream.h>

#include "../ConcurrentState.h"
#include "sys_cmd.h"

/**
 *
 * RPCClient[N1] -- RPCRouter - RPCServer[N2]
 *
 *
 *
 *
 */

namespace zertcore { namespace concurrent { namespace rpc {
using namespace zertcore::db;
using namespace zertcore::utils;

/**
 * use msgpack as RPC serializer & unserializer as default(depre)
 * use BSON since it support bin data
 */
/**
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
*/

#ifndef ZC_RPC_ISTREAM_TYPE
typedef mongodb::serialization::BSONIStream	istream_type;
#else
typedef ZC_RPC_ISTREAM_TYPE					istream_type;
#endif

#ifndef ZC_RPC_OSTREAM_TYPE
typedef mongodb::serialization::BSONOStream	ostream_type;
#else
typedef ZC_RPC_OSTREAM_TYPE					ostream_type;
#endif

#ifndef ZC_RPC_KEY_TYPE
typedef string								key_type;
#else
typedef ZC_RPC_KEY_TYPE						key_type;
#endif

typedef serialization::Serializer<istream_type>
											iarchiver_type;
typedef serialization::Unserializer<ostream_type>
											oarchiver_type;

}}}

namespace zertcore { namespace concurrent { namespace rpc {

typedef ThreadHandler<void (key_type, oarchiver_type params,
		iarchiver_type& ret_data)>			rpc_handler_type;
/**
typedef th_rpc_type::function_type			rpc_handler_type;
*/
typedef ThreadHandler<void (key_type, Error, oarchiver_type)>
											rpc_callback_type;
/**
typedef th_rpc_callback_type::function_type	rpc_callback_type;
*/
typedef ThreadHandler<void (key_type, oarchiver_type params)>
											data_sync_handler_type;
/**
typedef	th_data_sync_handler_type::function_type
											data_sync_handler_type;
*/

typedef uuid_t								rpc_id_t;

/**
 *
 */
const static u32 RPC_MAX_PACKAGE_SIZE		= 48 * 1024 * 1024;
}}}

namespace zertcore { namespace concurrent { namespace rpc {

typedef ConditionGroup<key_type, oarchiver_type>
											condition_group;
typedef condition_group::element_type		condition;

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
	uuid_t						id{0};
	RemoteConfig				rc;

	/**
	 * binds remote call list
	 */
	set<key_type>				call_keys;

	template <class Archiver>
	void serialize(Archiver& archiver) const {
		archiver & rc;
		archiver["id"] & id;
		archiver["call_keys"] & call_keys;
	}

	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		ZC_ASSERT(archiver & rc);
		ZC_ASSERT(archiver["id"] & id);
		ZC_ASSERT(archiver["call_keys"] & call_keys);
		return true;
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
	unordered_map<key_type, condition_group>
								keys;

	template <class Archiver>
	void serialize(Archiver& archiver) const {
		archiver & rc;
		archiver["keys"] & keys;
	}

	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		/**
		 * keys maight be empty
		 */
		archiver["keys"] & keys;
		return (archiver & rc);
	}
};

}}}

#include "sys_cmd.h"

#endif /* CONFIG_H_ */
