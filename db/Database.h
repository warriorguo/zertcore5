/*
 * Database.h
 *
 *  Created on: 2014Äê4ÔÂ30ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_DATABASE_H_
#define ZERTCORE_DATABASE_H_

#include <db/DatabaseInterface.h>
#include <db/DatabaseInterfaceManager.h>

#ifndef ZC_DISABLE_MONGODB

#include <db/mongodb/MongoDBAdapter.h>
#include <db/mongodb/MongoDBCursor.h>
#include <db/mongodb/MongoDBQuery.h>

namespace zertcore { namespace db {

namespace mongo {
typedef DatabaseInterfaceManager<MongoDBAdapter, MongoDBCursor, BSONOArchiver,
		BSONIArchiver, const MongoDBAdapterConfig, MongoDBQuery>
											DatabaseManager;
typedef DatabaseManager::database_type		Database;
}

}}

#endif

#endif /* DATABASE_H_ */
