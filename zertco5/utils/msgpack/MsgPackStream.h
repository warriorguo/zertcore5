/*
 * MsgPackStream.h
 *
 *  Created on: 2015��1��22��
 *      Author: Administrator
 */

#ifndef ZERTCORE_MSGPACKSTREAM_H_
#define ZERTCORE_MSGPACKSTREAM_H_

#include <pch.h>
#include <utils/types.h>
#include <log/Log.h>

#include <serialize/config.h>

#include "msgpack_stream_intend.h"

namespace zertcore { namespace utils { namespace messagepack {
using namespace zertcore::serialization;
using namespace msgpack;

typedef ::msgpack::object				msgobj;
}}}

namespace zertcore { namespace utils { namespace messagepack {

/**
 * MsgPackIStream
 */
class MsgPackIStream : noncopyable
{
public:
	static const u32						OBJECT_INIT_SIZE = 1024;

public:
	MsgPackIStream() : type_(TYPE_NONE) {}

public:
	void setListSize(const size_t& size) {
		ZC_ASSERT(type_ == TYPE_ARRAY);
		setupObject(size);
	}
	void setObjectSize(const size_t& size) {
		ZC_ASSERT(type_ == TYPE_OBJECT);
		setupObject(size);
	}

public:
	template <typename T>
	void addList(const T& v) {
		if (obj_.via.array.size >= obj_.size) {
			setupObject(obj_.size + OBJECT_INIT_SIZE);
		}

		obj_.via.array.ptr[obj_.via.array.size] << v;
		obj_.via.array.size++;
	}
	template <typename T>
	void addObject(const key_type& key, const T& v) {
		if (obj_.via.map.size >= obj_.size) {
			setupObject(obj_.size + OBJECT_INIT_SIZE);
		}

		obj_.via.map.ptr[obj_.via.map.size].key << key;
		obj_.via.map.ptr[obj_.via.map.size].val << v;
		obj_.via.map.size++;
	}

public:
	void setType(const value_type& t) {
		type_ = t;

		switch(type_) {
		case TYPE_OBJECT:
			obj_.type = ::msgpack::type::MAP;
			break;
		case TYPE_ARRAY:
			obj_.type = ::msgpack::type::ARRAY;
			break;

		default:
			ZCLOG(FINAL) << "Unknown type:" << t << End;
			break;
		}
	}

public:
	SharedBuffer buffer() const;
	zc_safe_input_object data() const;

public:
	void setupObject(const u32& size) {
		if (obj_.size >= size)
			return ;

		obj_.size = size;
		obj_.alloc();
	}

private:
	u32							type_;

	zc_safe_input_object		obj_;
};

}}}

namespace zertcore { namespace utils { namespace messagepack {

class MsgPackOStream : noncopyable
{
public:
	typedef u32								iterator_type;

public:
	MsgPackOStream() : type_(TYPE_NONE) {}

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

	bool getValue(const key_type& key, msgobj& value);
	bool getValue(iterator_type& it, key_type& key, msgobj& value);

public:
	value_type getType() const {
		return type_;
	}

public:
	bool buffer(const SharedBuffer& source);
	bool data(const msgobj& d);
	msgobj& data() {return data_;}
	bool initData();

private:
	value_type					type_;

	output_object_map_type		object_;
	output_object_array_type	array_;
	msgobj						data_;
};

}}}

namespace zertcore { namespace utils {
typedef messagepack::MsgPackIStream			MsgpackIStream;
typedef messagepack::MsgPackOStream			MsgpackOStream;
}}

#endif /* MSGPACKSTREAM_H_ */
