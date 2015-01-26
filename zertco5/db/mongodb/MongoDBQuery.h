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

#include <serialize/Serializer.h>
#include "serialize/BSONStream.h"

namespace zertcore{ namespace db{ namespace mongodb{
using namespace zertcore::serialization;
using namespace zertcore::db::mongodb::serialization;
using namespace zertcore::db::query;
}}}

namespace zertcore{ namespace db{ namespace mongodb{

class MongoDBQuery :
		public Query<Serializer<BSONIStream> >
{
public:
	typedef Query<Serializer<BSONIStream> >	base_type;

public:
	template <typename T>
	MongoDBQuery& key(const T& v) {
		data_["_id"] << v;
		return *this;
	}
	template <typename T>
	MongoDBQuery& equ(const key_type& key, const T& v) {
		data_[key].setOpCode(OP_EQU) << v;
		return *this;
	}
	template <typename T>
	MongoDBQuery& ne(const key_type& key, const T& v) {
		data_[key].setOpCode(OP_NE) << v;
		return *this;
	}
	template <typename T>
	MongoDBQuery& gt(const key_type& key, const T& v) {
		data_[key].setOpCode(OP_GT) << v;
		return *this;
	}
	template <typename T>
	MongoDBQuery& gte(const key_type& key, const T& v) {
		data_[key].setOpCode(OP_GTE) << v;
		return *this;
	}
	template <typename T>
	MongoDBQuery& lt(const key_type& key, const T& v) {
		data_[key].setOpCode(OP_LT) << v;
		return *this;
	}
	template <typename T>
	MongoDBQuery& lte(const key_type& key, const T& v) {
		data_[key].setOpCode(OP_LTE) << v;
		return *this;
	}
	template <typename T>
	MongoDBQuery& in(const key_type& key, const set<T>& v) {
		data_[key].setOpCode(OP_IN) << v;
		return *this;
	}
	template <typename T>
	MongoDBQuery& nin(const key_type& key, const set<T>& v) {
		data_[key].setOpCode(OP_NIN) << v;
		return *this;
	}

public:
	MongoDBQuery& andCond(MongoDBQuery& query) {
		data_.setOpCode(OP_AND).setSerializer(query.data_);
		return *this;
	}
	MongoDBQuery& orCond(MongoDBQuery& query) {
		data_.setOpCode(OP_OR).setSerializer(query.data_);
		return *this;
	}

};

}}}

#endif /* MONGODBQUERY_H_ */
