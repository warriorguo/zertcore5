/*
 * MongoDBDataProvider.h
 *
 *  Created on: 2014��11��24��
 *      Author: Administrator
 */

#ifndef ZERTCORE_MONGODBDATAPROVIDER_H_
#define ZERTCORE_MONGODBDATAPROVIDER_H_

#include <pch.h>

#include <db/io/DataProvider.h>
#include <db/Database.h>

namespace zertcore { namespace db { namespace io {
using namespace zertcore::db::mongo;
}}}

namespace zertcore { namespace db { namespace io {

/**
 * MongoDBDataProvider<Object>
 */
template <class Object>
class MongoDBDataProvider :
		public DataProvider<Object>
{
	ZC_TO_STRING("MongoDBDataProvider");
public:
	virtual ~MongoDBDataProvider() {}

public:
	virtual bool serialize(object_ptr object, const id_type& id) {
		mongo::Database::ptr db = mongo::DatabaseManager::Instance().fetchById(id);
		if (!db)
			return false;

		BSONIArchiver ar;
		ar & *object;

		return db->replace(Object::TABLE_NAME, ar, id);
	}
	virtual bool unserialize(object_ptr object, const id_type& id) {
		mongo::Database::ptr db = mongo::DatabaseManager::Instance().fetchById(id);
		if (!db)
			return false;

		BSONOArchiver ar;

		if (!db->queryOne(Object::TABLE_NAME, ar, id)) {
			return false;
		}

		return ar & *object;
	}
	virtual ThreadIndex getThreadIndex(const id_type& id) {
		return mongo::DatabaseManager::Instance().threadIndex(id);
	}


};

}}}


#endif /* MONGODBDATAPROVIDER_H_ */
