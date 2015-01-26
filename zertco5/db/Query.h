/*
 * DatabaseQuery.h
 *
 *  Created on: 2014Äê5ÔÂ3ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_DATABASEQUERY_H_
#define ZERTCORE_DATABASEQUERY_H_

#include <pch.h>

#include <utils/types.h>
#include <utils/time/TimeType.h>

#include <serialize/config.h>

namespace zertcore{ namespace db{ namespace query{
using namespace zertcore::serialization;

/**
 * Query for Database Query action, basically for query & update & remove
 */
template <typename DataPackage>
class Query
{
public:
	typedef DataPackage						data_type;

public:
	Query() : limit_(0), from_(0) {}

	template <typename T>
	Query(const T& v) : limit_(0), from_(0) { key(v); }

public:
	Query& andCond(const Query& query);
	Query& orCond(const Query& query);

public:
	Query& operator&& (const Query& query) {
		return andCond(query);
	}
	Query& operator|| (const Query& query) {
		return orCond(query);
	}

public:
	void setLimit(uint limit) {
		limit_ = limit;
	}
	void setFrom(uint from) {
		from_ = from;
	}

	uint getLimit() const {
		return limit_;
	}
	uint getFrom() const {
		return from_;
	}

public:
	template <typename T>
	Query<data_type>& key(const T& v);

	template <typename T>
	Query<data_type>& equ(const key_type& key, const T& v);
	template <typename T>
	Query<data_type>& ne(const key_type& key, const T& v);

	template <typename T>
	Query<data_type>& gt(const key_type& key, const T& v);
	template <typename T>
	Query<data_type>& gte(const key_type& key, const T& v);

	template <typename T>
	Query<data_type>& lt(const key_type& key, const T& v);
	template <typename T>
	Query<data_type>& lte(const key_type& key, const T& v);

	template <typename T>
	Query<data_type>& in(const key_type& key, const set<T>& v);
	template <typename T>
	Query<data_type>& nin(const key_type& key, const set<T>& v);

public:
	const data_type& getData() const {return data_;}

protected:
	uint						limit_;
	uint						from_;

protected:
	data_type					data_;
};


}}}


#endif /* DATABASEQUERY_H_ */
