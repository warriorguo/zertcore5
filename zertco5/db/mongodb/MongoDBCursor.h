/*
 * MongoDBCursor.h
 *
 *  Created on: 2014��11��17��
 *      Author: Administrator
 */

#ifndef ZERTCORE_MONGODBCURSOR_H_
#define ZERTCORE_MONGODBCURSOR_H_

#include <pch.h>

#include <object/PoolObject.h>
#include <serialize/Unserializer.h>

#include "config.h"
#include "serialize/BSONStream.h"

namespace zertcore{ namespace db{ namespace mongodb{
using namespace zertcore::object;
using namespace zertcore::db::mongodb::serialization;
}}}

namespace zertcore{ namespace db{ namespace mongodb{

/**
 * MongoDBCursor
 */
class MongoDBCursor :
		public PoolObject<MongoDBCursor>
{
public:
	typedef SMART_PTR(MongoDBCursor)		ptr;

	ZC_TO_STRING("MongoDBCursor");
public:
	MongoDBCursor();
	virtual ~MongoDBCursor();

public:
	/**
	 * return true if got data and moved to next one
	 */
	bool moveNext(Unserializer<BSONOStream>& data);

public:
	void setCursor(auto_ptr<DBClientCursor> cursor) {
		cursor_ = cursor;
	}

private:
	auto_ptr<DBClientCursor>	cursor_;
};

}}}

#endif /* MONGODBCURSOR_H_ */
