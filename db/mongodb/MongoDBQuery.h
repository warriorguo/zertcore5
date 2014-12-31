/*
 * MongoDBQuery.h
 *
 *  Created on: 2014Äê11ÔÂ17ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_MONGODBQUERY_HPP_
#define ZERTCORE_MONGODBQUERY_HPP_

#include <pch.h>
#include <db/Query.h>

#include <db/mongodb/serialize/BSONArchiver.h>

namespace zertcore{ namespace db{ namespace mongo{
using namespace zertcore::db::mongo::serialization;
using namespace zertcore::db::query;
}}}

namespace zertcore{ namespace db{ namespace mongo{

class MongoDBQuery :
		public Query<BSONIArchiver>
{
public:
	template <typename T>
	MongoDBQuery& key(const T& v) {
		data_("_id") << v;
		return *this;
	}
	template <typename T>
	MongoDBQuery& equ(const key_type& key, const T& v) {
		data_(key) << v;
		return *this;
	}
	template <typename T>
	MongoDBQuery& ne(const key_type& key, const T& v) {
		data_(key) << NE << v;
		return *this;
	}
	template <typename T>
	MongoDBQuery& gt(const key_type& key, const T& v) {
		data_(key) << GT << v;
		return *this;
	}
	template <typename T>
	MongoDBQuery& gte(const key_type& key, const T& v) {
		data_(key) << GTE << v;
		return *this;
	}
	template <typename T>
	MongoDBQuery& lt(const key_type& key, const T& v) {
		data_(key) << LT << v;
		return *this;
	}
	template <typename T>
	MongoDBQuery& lte(const key_type& key, const T& v) {
		data_(key) << LTE << v;
		return *this;
	}
	template <typename T>
	MongoDBQuery& in(const key_type& key, const set<T>& v) {
		data_(key) << "$in" << v;
		return *this;
	}
	template <typename T>
	MongoDBQuery& nin(const key_type& key, const set<T>& v) {
		data_(key) << "$nin" << v;
		return *this;
	}

};

}}}
/**
namespace zertcore{ namespace db{ namespace query{

template <typename T>
Query<BSONIArchiver>& Query<BSONIArchiver>::key(const T& v) {
	data_("_id") << v;
	return *this;
}

template <typename T>
Query<BSONIArchiver>& Query<BSONIArchiver>::equ(const key_type& key, const T& v) {
	data_(key) << v;
	return *this;
}

template <typename T>
Query<BSONIArchiver>& Query<BSONIArchiver>::ne(const key_type& key, const T& v) {
	data_(key) << NE << v;
	return *this;
}

template <typename T>
Query<BSONIArchiver>& Query<BSONIArchiver>::gt(const key_type& key, const T& v) {
	data_(key) << GT << v;
	return *this;
}

template <typename T>
Query<BSONIArchiver>& Query<BSONIArchiver>::gte(const key_type& key, const T& v) {
	data_(key) << GTE << v;
	return *this;
}

template <typename T>
Query<BSONIArchiver>& Query<BSONIArchiver>::lt(const key_type& key, const T& v) {
	data_(key) << LT << v;
	return *this;
}

template <typename T>
Query<BSONIArchiver>& Query<BSONIArchiver>::lte(const key_type& key, const T& v) {
	data_(key) << LTE << v;
	return *this;
}

template <typename T>
Query<BSONIArchiver>& Query<BSONIArchiver>::in(const key_type& key, const set<T>& v) {
	data_(key) << "$in" << v;
	return *this;
}

template <typename T>
Query<BSONIArchiver>& Query<BSONIArchiver>::nin(const key_type& key, const set<T>& v) {
	data_(key) << "$nin" << v;
	return *this;
}

}}}
*/
#endif /* MONGODBQUERY_H_ */
