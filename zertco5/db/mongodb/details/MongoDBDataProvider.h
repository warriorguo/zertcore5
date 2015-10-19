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

#include <thread/Thread.h>

#include <serialize/Serializer.h>
#include <serialize/Unserializer.h>

#include "../serialize/BSONStream.h"

namespace zertcore { namespace db { namespace io {
using namespace zertcore::db::mongodb;
using namespace zertcore::concurrent;
using namespace zertcore::serialization;
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
	virtual bool serialize(typename ActiveObjectTraits<Object>::ptr object,
			const typename ActiveObjectTraits<Object>::id_type& id) {
		mongodb::Database::ptr db = mongodb::DatabaseManager::Instance().fetchById(id);
		if (!db)
			return false;

		Serializer<BSONIStream> ar;
		ar & *object;

		mongodb::Database::query_type q(id);
		return db->replace(ActiveObjectTraits<Object>::TABLE_NAME, ar, q);
	}
	virtual bool unserialize(typename ActiveObjectTraits<Object>::ptr object,
			const typename ActiveObjectTraits<Object>::id_type& id) {
		mongodb::Database::ptr db = mongodb::DatabaseManager::Instance().fetchById(id);
		if (!db)
			return false;

		Unserializer<BSONOStream> ar;

		mongodb::Database::query_type q(id);
		if (!db->queryOne(ActiveObjectTraits<Object>::TABLE_NAME, ar, q)) {
			ZCLOG(ERROR) << "Not found for id=" << id << End;
			return false;
		}

		return ar & *object;
	}

public:
	virtual tid_type getThreadIndex() const {
		return Thread::lazyTid(details::RUNNING_THREAD_ID);
	}


};

}}}


#endif /* MONGODBDATAPROVIDER_H_ */
