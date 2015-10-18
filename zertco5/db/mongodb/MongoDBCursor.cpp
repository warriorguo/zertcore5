/*
 * MongoDBCursor.cpp
 *
 *  Created on: 2014��11��17��
 *      Author: Administrator
 */

#include <pch.h>
#include <details.h>

#include "MongoDBCursor.h"

namespace zertcore{ namespace db{ namespace mongodb{

MongoDBCursor::MongoDBCursor() {
	// TODO Auto-generated constructor stub
}

MongoDBCursor::~MongoDBCursor() {
	// TODO Auto-generated destructor stub
}

bool MongoDBCursor::moveNext(Unserializer<BSONOStream>& data) {
	if (cursor_->more()) {
		data.stream().data(cursor_->next());
		return true;
	}
	return false;
}


}}}
