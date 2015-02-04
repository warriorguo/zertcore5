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
typedef ::msgpack::zone					msgzone;
typedef ::msgpack::object::with_zone	msgobj_zone;
}}}

namespace zertcore { namespace utils { namespace messagepack {

/**
 * MsgPackIStream
 */
class MsgPackIStream : noncopyable
{
public:
	typedef SMART_PTR(msgzone)				zone_ptr;
	typedef SMART_PTR(char[])				buffer_ptr;

public:
	static const u32						OBJECT_INIT_SIZE = 512;

public:
	MsgPackIStream();
	MsgPackIStream(MsgPackIStream& stream);

public:
	void setListSize(const size_t& size) {
		ZC_ASSERT(type_ == TYPE_ARRAY);
		setupBuffer(size);
	}
	void setObjectSize(const size_t& size) {
		ZC_ASSERT(type_ == TYPE_OBJECT);
		setupBuffer(size);
	}

public:
	/**
	 * if value was object, didnt need zone to store the value again
	 */
	void addList(const msgobj& val);
	void addObject(const key_type& key, const msgobj& val);

	template <typename T>
	void addList(const T& val) {
		if (obj_.via.array.size >= buffer_size_) {
			setupBuffer(buffer_size_ + OBJECT_INIT_SIZE);
		}

		msgobj& v = obj_.via.array.ptr[obj_.via.map.size];
		new (&v) msgobj(val, *zone_);
		obj_.via.array.size++;
	}
	template <typename T>
	void addObject(const key_type& key, const T& val) {
		if (obj_.via.map.size >= buffer_size_) {
			setupBuffer(buffer_size_ + OBJECT_INIT_SIZE);
		}

		msgobj& k = obj_.via.map.ptr[obj_.via.map.size].key;
		msgobj& v = obj_.via.map.ptr[obj_.via.map.size].val;
		new (&k) msgobj(key, *zone_);
		new (&v) msgobj(val, *zone_);

		obj_.via.map.size++;
	}

public:
	void setType(const value_type& t);

public:
	SharedBuffer buffer() const;
	msgobj data() const;

private:
	void setupBuffer(const u32& size);

private:
	u32							type_;
	msgobj						obj_;

	zone_ptr					zone_;
	buffer_ptr					buffer_;

	u32							buffer_size_;
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
	iterator_type begin() {return 0;}

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
	unpacked					unpacker_;
};

}}}

namespace zertcore { namespace utils {
typedef messagepack::MsgPackIStream			MsgpackIStream;
typedef messagepack::MsgPackOStream			MsgpackOStream;
}}

#endif /* MSGPACKSTREAM_H_ */
