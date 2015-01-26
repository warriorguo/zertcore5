/*
 * MsgPackStream.h
 *
 *  Created on: 2015Äê1ÔÂ22ÈÕ
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
}}}

namespace zertcore { namespace utils { namespace messagepack {

/**
 * MsgPackIStream
 */
class MsgPackIStream : noncopyable
{
public:
	enum {
		TYPE_NONE							= 0,
		TYPE_ARRAY							= 1,
		TYPE_OBJECT							= 2,
	};

public:
	MsgPackIStream() : type_(TYPE_NONE) {}

public:
	template <typename T>
	void setValue(const key_type& key, const T& v, const op_code_type& op_code) {
		/** ignore the op code **/
		if (type_ == TYPE_OBJECT)
			input_object_.insert(input_object_map_type::value_type(key, zc_safe_input_object(v)));
		else
			input_array_.push_back(zc_safe_input_object(v));
	}

	void setValue(const key_type& key, const char* v);
	void setStream(const key_type& key, MsgPackIStream& stream, const op_code_type& op_code);

public:
	void setObject() {type_ = TYPE_OBJECT;}
	void setArray() {type_ = TYPE_ARRAY;}

public:
	string str() const;
	zc_safe_input_object data() const;

private:
	u32							type_;

private:
	mutable string				result_;
	input_object_map_type		input_object_;
	input_object_array_type		input_array_;
};

}}}

namespace zertcore { namespace utils { namespace messagepack {

class MsgPackOStream : noncopyable
{
public:
	enum {
		TYPE_NONE							= 0,
		TYPE_ARRAY							= 1,
		TYPE_OBJECT							= 2,
	};

public:
	typedef pair<key_type, ::msgpack::object>
											value_type;

public:
	MsgPackOStream() : type_(TYPE_OBJECT), parse_type_(TYPE_NONE),
		array_index_(0) {}

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
	bool nextObject(value_type& it);

public:
	bool getStream(key_type& key, MsgPackOStream& stream);

public:
	bool str(const string& source);
	bool data(const ::msgpack::object& d);

private:
	u32							type_;
	u32							parse_type_;

	output_object_map_type		object_;
	output_object_map_type::iterator
								obj_iter_;

	output_object_array_type	array_;
	size_t						array_index_;
};

}}}

namespace zertcore { namespace utils {
typedef messagepack::MsgPackIStream			MsgpackIStream;
typedef messagepack::MsgPackOStream			MsgpackOStream;
}}

#endif /* MSGPACKSTREAM_H_ */
