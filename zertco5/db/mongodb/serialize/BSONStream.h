/*
 * BSONStream.h
 *
 *  Created on: 2015Äê1ÔÂ19ÈÕ
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
 * BSONIStream
 */
class BSONIStream : noncopyable
{
public:
	enum {
		TYPE_NONE							= 0,
		TYPE_ARRAY							= 1,
		TYPE_OBJECT							= 2,
	};

public:
	BSONIStream() : type_(TYPE_NONE), or_flag_(false) {}

public:
	template <typename T>
	void setValue(const key_type& key, const T& v, const op_code_type& op_code) {
		if (type_ == TYPE_ARRAY) {
			array_.append(v);
			return ;
		}

		ZC_ASSERT(type_ == TYPE_OBJECT);

		if (!op_code) {
			obj_.append(key, v);
			return ;
		}

		switch(op_code) {
		case OP_EQU:
			obj_ << key << v;
			break;
		case OP_NE:
			obj_ << key << NE << v;
			break;
		case OP_GT:
			obj_ << key << GT << v;
			break;
		case OP_GTE:
			obj_ << key << GTE << v;
			break;
		case OP_LT:
			obj_ << key << LT << v;
			break;
		case OP_LTE:
			obj_ << key << LTE << v;
			break;

		default:
			ZCLOG(FINAL) << "Unsopprt OPCODE:" << (u32)op_code << End;
		}
	}

	void setValue(const key_type& key, const char* v);
	void setStream(const key_type& key, BSONIStream& stream, const op_code_type& op_code);

public:
	void setObject() {type_ = TYPE_OBJECT;}
	void setArray() {type_ = TYPE_ARRAY;}

public:
	string str() const;
	BSONObj data() const;

public:
	BSONObjBuilder& getObjBuilder() {
		ZC_ASSERT(type_ == TYPE_OBJECT);
		return obj_;
	}
	BSONArrayBuilder& getArrayBuilder() {
		ZC_ASSERT(type_ == TYPE_ARRAY);
		return array_;
	}

private:
	u32							type_;

private:
	mutable BSONObj				result_;
	mutable BSONObjBuilder		obj_;
	mutable BSONArrayBuilder	array_;
	bool						or_flag_;
};


/**
 * BSONOStream
 */
class BSONOStream : noncopyable
{
public:
	enum {
		TYPE_NONE							= 0,
		TYPE_ARRAY							= 1,
		TYPE_OBJECT							= 2,
	};

public:
	BSONOStream() : type_(TYPE_NONE), iter_(data_) {}

public:
	bool getValue(key_type& key, i8& value);
	bool getValue(key_type& key, i16& value);
	bool getValue(key_type& key, i32& value);
	bool getValue(key_type& key, i64& value);

	bool getValue(key_type& key, u8& value);
	bool getValue(key_type& key, u16& value);
	bool getValue(key_type& key, u32& value);
	bool getValue(key_type& key, u64& value);

	bool getValue(key_type& key, f32& value);
	bool getValue(key_type& key, f64& value);

	bool getValue(key_type& key, bool& value);
	bool getValue(key_type& key, string& value);

public:
	void setObject() {type_ = TYPE_OBJECT;}
	void setArray() {type_ = TYPE_ARRAY;}

public:
	bool getStream(key_type& key, BSONOStream& stream);

public:
	bool str(const string& source);
	void data(const BSONObj& d);
	BSONObj data() const {return data_;}

private:
	u32							type_;
	BSONObj						data_;
	BSONObjIterator				iter_;
};

}}}}

#endif /* BSONSTREAM_H_ */
