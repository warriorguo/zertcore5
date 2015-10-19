/*
 * MongoDBConfig.h
 *
 *  Created on: 2014��11��19��
 *      Author: Administrator
 */

#ifndef ZERTCORE_MONGODBCONFIG_H_
#define ZERTCORE_MONGODBCONFIG_H_

#include <pch.h>
#include <utils/types.h>
#include <object/ObjectBase.h>

#include <mongo/client/dbclient.h>

namespace zertcore { namespace db { namespace mongodb {
using namespace mongo;
using namespace zertcore::object;
}}}


namespace zertcore { namespace db { namespace mongodb {

namespace details {

enum {
	RUNNING_THREAD_ID						= 4,
};

}

/**
 * MongoDBAdapterConfig
 */
struct MongoDBAdapterConfig :
		Serializable<MongoDBAdapterConfig>, Unserializable<MongoDBAdapterConfig>
{
	string						host{"127.0.0.1"};
	u32							port{27017};

	string						db;

	template <class Archiver>
	void serialize(Archiver& archiver) const {
		archiver["host"] & host;
		archiver["port"] & port;
		archiver["db"] & db;
	}
	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		return  (archiver["host"] & host) &&
				(archiver["port"] & port) &&
				(archiver["db"] & db);
	}
};

typedef vector<MongoDBAdapterConfig>		mongodb_config_list_type;

}}}

#endif /* MONGODBCONFIG_H_ */
