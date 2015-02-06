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

/**
 * MongoDBAdapterConfig
 */
struct MongoDBAdapterConfig :
		public ObjectBase<MongoDBAdapterConfig>
{
	string						host;
	u32							port{27017};

	ZC_TO_STRING("host" << host << "port" << port);
};


}}}

#endif /* MONGODBCONFIG_H_ */
