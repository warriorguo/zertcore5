/*
 * BSONStream.h
 *
 *  Created on: 2015��1��19��
 *      Author: Administrator
 */

#ifndef ZERTCORE_DB_BSONSTREAM_H_
#define ZERTCORE_DB_BSONSTREAM_H_

#include <pch.h>
#include <utils/types.h>
#include <log/Log.h>

#include <serialize/config.h>

#include "../config.h"

namespace zertcore { namespace db { namespace mongodb { namespace serialization {
using namespace zertcore::serialization;
}}}}

namespace zertcore { namespace db { namespace mongodb { namespace serialization {

/**
 * BSONIStream for Serializer
 */
class BSONIStream : noncopyable
{
public:
	BSONIStream() : index_(0), label_(NULL) {}
	BSONIStream(BSONIStream&) : index_(0), label_(NULL) {}

public:
	void setListSize(const size_t& size) {}
	void setObjectSize(const size_t& size) {}

public:
	template <typename T>
	void addList(const T& v) {
		addObject(lexical_cast<key_type>(index_++), v);
	}
	template <typename T>
	void addObject(const key_type& key, const T& v) {
		if (label_.l_) {
			obj_ << key << label_ << v;
			label_.l_ = NULL;
		}
		else
			obj_ << key << v;
	}

	void setLabel(const Labeler::Label& label) {
		label_ = label;
	}

public:
	void setType(const value_type& type) {}

public:
	const BSONObj& data() const {
		if (!result_.isEmpty())
			return result_;

		return result_ = obj_.obj();
	}
	BSONObj& data() {
		if (!result_.isEmpty())
			return result_;

		return result_ = obj_.obj();
	}
	bool initData() {return true;}

public:
	SharedBuffer buffer() const {
		SharedBuffer buf;
		buf.assign(data().jsonString());

		return buf;
	}

private:
	mutable BSONObj				result_;
	mutable BSONObjBuilder		obj_;


	u32							index_;
	/**
	 * extension for query action
	 */
	Labeler::Label				label_;
};

}}}}

namespace zertcore { namespace db { namespace mongodb { namespace serialization {

/**
 * BSONOStream for Unserializer
 */
class BSONOStream : noncopyable
{
public:
	typedef BSONObjIterator					iterator_type;

public:
	BSONOStream() {}

public:
	bool getValue(const key_type& key, i8& value);
	bool getValue(iterator_type& it, key_type& key, i8& value);

	bool getValue(const key_type& key, i16& value);
	bool getValue(iterator_type& it, key_type& key, i16& value);

	bool getValue(const key_type& key, i32& value);
	bool getValue(iterator_type& it, key_type& key, i32& value);

	bool getValue(const key_type& key, i64& value);
	bool getValue(iterator_type& it, key_type& key, i64& value);

	bool getValue(const key_type& key, u8& value);
	bool getValue(iterator_type& it, key_type& key, u8& value);

	bool getValue(const key_type& key, u16& value);
	bool getValue(iterator_type& it, key_type& key, u16& value);

	bool getValue(const key_type& key, u32& value);
	bool getValue(iterator_type& it, key_type& key, u32& value);

	bool getValue(const key_type& key, u64& value);
	bool getValue(iterator_type& it, key_type& key, u64& value);

	bool getValue(const key_type& key, f32& value);
	bool getValue(iterator_type& it, key_type& key, f32& value);

	bool getValue(const key_type& key, f64& value);
	bool getValue(iterator_type& it, key_type& key, f64& value);

	bool getValue(const key_type& key, bool& value);
	bool getValue(iterator_type& it, key_type& key, bool& value);

	bool getValue(const key_type& key, string& value);
	bool getValue(iterator_type& it, key_type& key, string& value);

	bool getValue(const key_type& key, BSONObj& value);
	bool getValue(iterator_type& it, key_type& key, BSONObj& value);

public:
	/**
	 * all bson was based on object, list was based on keys (0, 1, 2 ..)
	 */
	value_type getType() const {
		return TYPE_OBJECT;
	}

public:
	iterator_type begin() {return data_.begin();}

public:
	bool buffer(const SharedBuffer& buf);
	void data(const BSONObj& d);
	BSONObj& data() {return data_;}
	bool initData() {return true;}

private:
	BSONObj						data_;
};

}}}}

#endif /* BSONSTREAM_H_ */
