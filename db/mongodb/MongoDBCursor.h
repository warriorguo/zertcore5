/*
 * MongoDBCursor.h
 *
 *  Created on: 2014Äê11ÔÂ17ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_MONGODBCURSOR_H_
#define ZERTCORE_MONGODBCURSOR_H_

#include <pch.h>

#include <object/PoolObject.h>

#include <db/mongodb/MongoDBConfig.h>
#include <db/mongodb/serialize/BSONArchiver.h>

namespace zertcore{ namespace db{ namespace mongo{
using namespace zertcore::object;
using namespace zertcore::db::mongo::serialization;
}}}

namespace zertcore{ namespace db{ namespace mongo{

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
	bool moveNext(BSONOArchiver& data);

public:
	void setCursor(auto_ptr<DBClientCursor> cursor) {
		cursor_ = cursor;
	}

private:
	auto_ptr<DBClientCursor>	cursor_;
};

}}}

#endif /* MONGODBCURSOR_H_ */
