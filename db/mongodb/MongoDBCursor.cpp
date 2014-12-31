/*
 * MongoDBCursor.cpp
 *
 *  Created on: 2014Äê11ÔÂ17ÈÕ
 *      Author: Administrator
 */

#include <pch.h>
#include <db/mongodb/MongoDBCursor.h>

namespace zertcore{ namespace db{ namespace mongo{

MongoDBCursor::MongoDBCursor() {
	// TODO Auto-generated constructor stub
}

MongoDBCursor::~MongoDBCursor() {
	// TODO Auto-generated destructor stub
}

bool MongoDBCursor::moveNext(BSONOArchiver& data) {
	if (cursor_->more()) {
		data.setData(cursor_->next());
		return true;
	}

	return false;
}


}}}
