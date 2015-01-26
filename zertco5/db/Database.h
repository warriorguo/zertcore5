/*
 * Database.h
 *
 *  Created on: 2014Äê4ÔÂ30ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_DATABASE_H_
#define ZERTCORE_DATABASE_H_

#include "DatabaseInterface.h"
#include "DatabaseInterfaceManager.h"

#ifndef ZC_DISABLE_MONGODB

#include "mongodb/MongoDBAdapter.h"
#include "mongodb/MongoDBCursor.h"
#include "mongodb/MongoDBQuery.h"

namespace zertcore { namespace db {

namespace mongodb {
typedef DatabaseInterfaceManager<MongoDBAdapter>
											DatabaseManager;
typedef DatabaseManager::database_type		Database;
}

}}

#endif

#endif /* DATABASE_H_ */
