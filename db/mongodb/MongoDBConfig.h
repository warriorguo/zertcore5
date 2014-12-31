/*
 * MongoDBConfig.h
 *
 *  Created on: 2014Äê11ÔÂ19ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_MONGODBCONFIG_H_
#define ZERTCORE_MONGODBCONFIG_H_

#include <pch.h>
#include <utils/types.h>
#include <object/ObjectBase.h>

#include <mongo/client/dbclient.h>

namespace zertcore { namespace db { namespace mongo {
using namespace ::mongo;
using namespace zertcore::object;
}}}


namespace zertcore { namespace db { namespace mongo {

/**
 * MongoDBAdapterConfig
 */
struct MongoDBAdapterConfig :
		public ObjectBase<MongoDBAdapterConfig>
{
	string						host;
	u32							port;

	ZC_TO_STRING("host:" << host << ",port:" << port);
};


}}}

#endif /* MONGODBCONFIG_H_ */
