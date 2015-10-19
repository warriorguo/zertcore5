/*
 * MongoDBQuery.h
 *
 *  Created on: 2014��11��17��
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
	MongoDBQuery() : Query<Serializer<BSONIStream> >() {}

	template <typename T>
	MongoDBQuery(const T& v) : Query<Serializer<BSONIStream> >() { key(v); }

public:
	template <typename T>
	MongoDBQuery& key(const T& v) {
		data_["_id"] << v;
		return *this;
	}
	template <typename T>
	MongoDBQuery& equ(const key_type& key, const T& v) {

		data_[key] << v;
		return *this;
	}
	template <typename T>
	MongoDBQuery& ne(const key_type& key, const T& v) {
		data_[key]["$ne"] << v;
		return *this;
	}
	template <typename T>
	MongoDBQuery& gt(const key_type& key, const T& v) {
		data_[key]["$gt"] << v;
		return *this;
	}
	template <typename T>
	MongoDBQuery& gte(const key_type& key, const T& v) {
		data_[key]["$gte"] << v;
		return *this;
	}
	template <typename T>
	MongoDBQuery& lt(const key_type& key, const T& v) {
		data_[key]["$lt"] << v;
		return *this;
	}
	template <typename T>
	MongoDBQuery& lte(const key_type& key, const T& v) {
		data_[key]["$lte"] << v;
		return *this;
	}
	template <typename T>
	MongoDBQuery& in(const key_type& key, const set<T>& v) {
		data_[key]["$in"] << v;
		return *this;
	}
	template <typename T>
	MongoDBQuery& nin(const key_type& key, const set<T>& v) {
		data_[key]["$nin"] << v;
		return *this;
	}

public:
	MongoDBQuery& andCond(MongoDBQuery& query) {
		data_.stream().data() = BSONObjBuilder().
				appendElements(data_.stream().data()).
				appendElements(query.data_.stream().data()).
				obj();
		return *this;
	}
	MongoDBQuery& orCond(MongoDBQuery& query) {
		BSONElement elemt = data_.stream().data().getField("$or");
		if (elemt.eoo()) {
			or_conds_.push_back(data_.stream().data());
			or_conds_.push_back(query.data_.stream().data());

			data_.stream().data() = BSON(
					"$or" << BSON_ARRAY(data_.stream().data() << query.data_.stream().data()));
		}
		else {
			or_conds_.push_back(query.data_.stream().data());
			BSONArrayBuilder arr;
			for (u32 i = 0; i < or_conds_.size(); ++i) {
				arr << or_conds_[i];
			}
			data_.stream().data() = arr.obj();
		}

		return *this;
	}

private:
	vector<BSONObj>				or_conds_;
};

}}}

#endif /* MONGODBQUERY_H_ */
