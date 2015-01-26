/*
 * MsgPackStream.cpp
 *
 *  Created on: 2015Äê1ÔÂ22ÈÕ
 *      Author: Administrator
 */

#include "MsgPackStream.h"

namespace zertcore { namespace utils { namespace messagepack {
using namespace msgpack::type;
}}}

namespace zertcore { namespace utils { namespace messagepack {

void MsgPackIStream::setValue(const key_type& key, const char* v) {
	if (type_ == TYPE_OBJECT)
		input_object_.insert(input_object_map_type::value_type(key, zc_safe_input_object(v)));
	else
		input_array_.push_back(zc_safe_input_object(v));
}
void MsgPackIStream::setStream(const key_type& key, MsgPackIStream& stream, const op_code_type& op_code) {
	if (type_ == TYPE_OBJECT)
		input_object_.insert(input_object_map_type::value_type(key, stream.data()));
	else
		input_array_.push_back(stream.data());
}

zc_safe_input_object MsgPackIStream::data() const {
	if (type_ == TYPE_OBJECT)
		return zc_safe_input_object(input_object_);

	return zc_safe_input_object(input_array_);
}

string MsgPackIStream::str() const {
	if (!result_.empty())
		return result_;

	std::stringstream stream;

	pack(stream, (::msgpack::object)data());

	result_ = stream.str();
	return result_;
}

}}}

namespace zertcore { namespace utils { namespace messagepack {

bool MsgPackOStream::str(const string& source) {
	unpacked msg;
	try {
		unpack(&msg, source.data(), source.size());
	}
	catch(parse_error&) {
		::printf("unpack failed\n");
		return false;
	}

	return data(msg.get());
}

bool MsgPackOStream::data(const ::msgpack::object& d) {
	if (d.type == object_type::MAP) {
		parse_type_ = TYPE_OBJECT;
		d.convert(object_);
		obj_iter_ = object_.begin();

		return true;
	}
	else if (d.type == object_type::ARRAY) {
		parse_type_ = TYPE_ARRAY;
		d.convert(array_);
		array_index_ = 0;

		return true;
	}

	::printf("data failed:%d\n", (int)d.type);
	return false;
}

bool MsgPackOStream::getStream(key_type& key, MsgPackOStream& stream) {
	value_type v;
	v.first = key;

	if (!nextObject(v))
		return false;

	stream.parse_type_ = TYPE_NONE;
	stream.data(v.second);

	return stream.parse_type_ != TYPE_NONE;
}

bool MsgPackOStream::nextObject(value_type& value) {
	if (parse_type_ == TYPE_NONE) {
		return false;
	}

	if (type_ == TYPE_OBJECT) {
		if (parse_type_ == TYPE_OBJECT) {
			output_object_map_type::const_iterator it = object_.find(value.first);
			if (it == object_.end())
				return false;

			value = *it;
			return true;
		}

		// Can't use key to search in array
		return false;

		if (obj_iter_ == object_.end())
			return false;

		value = *obj_iter_;
		++obj_iter_;

		return true;
	}

	if (parse_type_ == TYPE_OBJECT) {
		if (obj_iter_ == object_.end())
			return false;

		value = *obj_iter_;
		++obj_iter_;

		return true;
	}

	if (array_index_ >= array_.size())
		return false;

	value.second = array_[array_index_++];
	return true;
}

bool MsgPackOStream::getValue(key_type& key, i8& value) {
	value_type v;
	v.first = key;

	if (!nextObject(v))
		return false;

	key = v.first;

	switch(v.second.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = v.second.via.boolean;
		break;

	case object_type::STR:
		value = lexical_cast<i8>(v.second.via.str.ptr);
		break;

	default:
		return false;
	}

	return true;
}
bool MsgPackOStream::getValue(key_type& key, i16& value) {
	value_type v;
	v.first = key;

	if (!nextObject(v))
		return false;

	key = v.first;

	switch(v.second.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (i16)v.second.via.i64;
		break;

	case object_type::STR:
		value = lexical_cast<i16>(v.second.via.str.ptr);
		break;

	default:
		return false;
	}

	return true;
}
bool MsgPackOStream::getValue(key_type& key, i32& value) {
	value_type v;
	v.first = key;

	if (!nextObject(v))
		return false;

	key = v.first;

	switch(v.second.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (i32)v.second.via.i64;
		break;

	case object_type::STR:
		value = lexical_cast<i32>(v.second.via.str.ptr);
		break;

	default:
		return false;
	}

	return true;
}
bool MsgPackOStream::getValue(key_type& key, i64& value) {
	value_type v;
	v.first = key;

	if (!nextObject(v))
		return false;

	key = v.first;

	switch(v.second.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (i64)v.second.via.i64;
		break;

	case object_type::STR:
		value = lexical_cast<i64>(v.second.via.str.ptr);
		break;

	default:
		return false;
	}

	return true;
}

bool MsgPackOStream::getValue(key_type& key, u8& value) {
	value_type v;
	v.first = key;

	if (!nextObject(v))
		return false;

	key = v.first;

	switch(v.second.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (u8)v.second.via.u64;
		break;

	case object_type::STR:
		value = lexical_cast<u8>(v.second.via.str.ptr);
		break;

	default:
		return false;
	}

	return true;
}
bool MsgPackOStream::getValue(key_type& key, u16& value) {
	value_type v;
	v.first = key;

	if (!nextObject(v))
		return false;

	key = v.first;

	switch(v.second.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (u16)v.second.via.u64;
		break;

	case object_type::STR:
		value = lexical_cast<u16>(v.second.via.str.ptr);
		break;

	default:
		return false;
	}

	return true;
}
bool MsgPackOStream::getValue(key_type& key, u32& value) {
	value_type v;
	v.first = key;

	if (!nextObject(v))
		return false;

	key = v.first;

	switch(v.second.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (u32)v.second.via.u64;
		break;

	case object_type::STR:
		value = lexical_cast<u32>(v.second.via.str.ptr);
		break;

	default:
		return false;
	}

	return true;
}
bool MsgPackOStream::getValue(key_type& key, u64& value) {
	value_type v;
	v.first = key;

	if (!nextObject(v))
		return false;

	key = v.first;

	switch(v.second.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (u64)v.second.via.u64;
		break;

	case object_type::STR:
		value = lexical_cast<u64>(v.second.via.str.ptr);
		break;

	default:
		return false;
	}

	return true;
}

bool MsgPackOStream::getValue(key_type& key, f32& value) {
	value_type v;
	v.first = key;

	if (!nextObject(v))
		return false;

	key = v.first;

	switch(v.second.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (f32)v.second.via.f64;
		break;

	case object_type::STR:
		value = lexical_cast<f32>(v.second.via.str.ptr);
		break;

	default:
		return false;
	}

	return true;
}
bool MsgPackOStream::getValue(key_type& key, f64& value) {
	value_type v;
	v.first = key;

	if (!nextObject(v))
		return false;

	key = v.first;

	switch(v.second.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (f64)v.second.via.f64;
		break;

	case object_type::STR:
		value = lexical_cast<f64>(v.second.via.str.ptr);
		break;

	default:
		return false;
	}

	return true;
}

bool MsgPackOStream::getValue(key_type& key, bool& value) {
	value_type v;
	v.first = key;

	if (!nextObject(v))
		return false;

	key = v.first;

	switch(v.second.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = v.second.via.boolean;
		break;

	case object_type::STR:
		value = v.second.via.str.size > 0;
		break;

	default:
		return false;
	}

	return true;
}
bool MsgPackOStream::getValue(key_type& key, string& value) {
	value_type v;
	v.first = key;

	if (!nextObject(v))
		return false;

	key = v.first;

	switch(v.second.type) {
	case object_type::BOOLEAN:
		value = v.second.via.boolean? "true": "false";
		break;
	case object_type::POSITIVE_INTEGER:
		value = lexical_cast<string>(v.second.via.u64);
		break;
	case object_type::NEGATIVE_INTEGER:
		value = lexical_cast<string>(v.second.via.i64);
		break;
	case object_type::FLOAT:
		value = lexical_cast<string>(v.second.via.f64);
		break;

	case object_type::STR:
	case object_type::BIN:
		value.assign(v.second.via.str.ptr, v.second.via.str.size);
		break;

	default:
		return false;
	}

	return true;
}

}}}
