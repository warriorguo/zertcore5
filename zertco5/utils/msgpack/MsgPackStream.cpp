/*
 * MsgPackStream.cpp
 *
 *  Created on: 2015��1��22��
 *      Author: Administrator
 */

#include "MsgPackStream.h"

namespace zertcore { namespace utils { namespace messagepack {
using namespace msgpack::type;
}}}

namespace zertcore { namespace utils { namespace messagepack {

SharedBuffer MsgPackIStream::buffer() const {
	sbuffer sbuf;
	pack(sbuf, obj_);

	SharedBuffer buf;
	buf.assign(sbuf.data(), sbuf.size());

	return buf;
}

void MsgPackIStream::setType(const value_type& t) {
	type_ = t;
	switch(type_) {
	case TYPE_OBJECT:
		obj_.type = object_type::MAP;
		break;
	case TYPE_ARRAY:
		obj_.type = object_type::ARRAY;
		break;

	default:
		break;
	}
}

void MsgPackIStream::setupBuffer(const u32& size) {
	if (size <= buffer_size_)
		return ;

	u32 m = 0;
	switch (type_) {
	case TYPE_OBJECT:
		m = sizeof(msgobj);
		break;
	case TYPE_ARRAY:
		m = sizeof(object_kv);
		break;

	default:
		return ;
	}

	char * buf = new char[size * m];
	if (buffer_size_ > 0) {
		::memcpy(buf, obj_.via.str.ptr, obj_.via.str.size * m);
	}

	buffer_size_ = size;
	obj_.via.str.ptr = buf;
	buffer_ = buffer_ptr(buf);
}

}}}

namespace zertcore { namespace utils { namespace messagepack {

bool MsgPackOStream::buffer(const SharedBuffer& source) {
	unpacked msg;
	try {
		unpack(msg, (const char *)source.data(), source.size());
	}
	catch(parse_error&) {
		::printf("unpack failed\n");
		return false;
	}

	return data(msg.get());
}

bool MsgPackOStream::data(const msgobj& d) {
	std::cout << d << std::endl;
	data_ = d;
	return initData();
}

bool MsgPackOStream::initData() {
	if (data_.type == object_type::MAP) {
		data_.convert(object_);
		type_ = TYPE_OBJECT;
	}
	else if (data_.type == object_type::ARRAY) {
		data_.convert(array_);
		type_ = TYPE_ARRAY;
	}
	else
		return false;

	return true;
}

bool MsgPackOStream::getValue(const key_type& key, i8& value) {
	if (type_ != TYPE_OBJECT)
		return false;

	output_object_map_type::iterator it = object_.find(key);
	if (it == object_.end())
		return false;

	switch(it->second.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (i8)it->second.via.i64;
		break;

	case object_type::STR:
		try {
			value = lexical_cast<i8>(it->second.via.str.ptr);
		}
		catch(bad_lexical_cast&) {
			return false;
		}
		break;

	default:
		return false;
	}

	return true;
}
bool MsgPackOStream::getValue(iterator_type& it, key_type& key, i8& value) {
	if (it >= data_.via.array.size) {
		return false;
	}

	msgobj value_obj;
	if (type_ == TYPE_OBJECT) {
		value_obj = data_.via.map.ptr[it].val;
		data_.via.map.ptr[it].key.convert(key);
	}
	else {
		value_obj = data_.via.array.ptr[it];
	}
	++it;

	switch(value_obj.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (i8)value_obj.via.i64;
		break;

	case object_type::STR:
		try {
			value = lexical_cast<i8>(value_obj.via.str.ptr);
		}
		catch(bad_lexical_cast&) {
			return false;
		}
		break;

	default:
		return false;
	}

	return true;
}

bool MsgPackOStream::getValue(const key_type& key, u8& value) {
	if (type_ != TYPE_OBJECT)
		return false;

	output_object_map_type::iterator it = object_.find(key);
	if (it == object_.end())
		return false;

	switch(it->second.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (u8)it->second.via.u64;
		break;

	case object_type::STR:
		try {
			value = lexical_cast<i8>(it->second.via.str.ptr);
		}
		catch(bad_lexical_cast&) {
			return false;
		}
		break;

	default:
		return false;
	}

	return true;
}
bool MsgPackOStream::getValue(iterator_type& it, key_type& key, u8& value) {
	if (it >= data_.via.array.size) {
		return false;
	}

	msgobj value_obj;
	if (type_ == TYPE_OBJECT) {
		value_obj = data_.via.map.ptr[it].val;
		data_.via.map.ptr[it].key.convert(key);
	}
	else {
		value_obj = data_.via.array.ptr[it];
	}
	++it;

	switch(value_obj.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (u8)value_obj.via.u64;
		break;

	case object_type::STR:
		try {
			value = lexical_cast<i8>(value_obj.via.str.ptr);
		}
		catch(bad_lexical_cast&) {
			return false;
		}
		break;

	default:
		return false;
	}

	return true;
}

bool MsgPackOStream::getValue(const key_type& key, i16& value) {
	if (type_ != TYPE_OBJECT)
		return false;

	output_object_map_type::iterator it = object_.find(key);
	if (it == object_.end())
		return false;

	switch(it->second.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (i16)it->second.via.i64;
		break;

	case object_type::STR:
		try {
			value = lexical_cast<i16>(it->second.via.str.ptr);
		}
		catch(bad_lexical_cast&) {
			return false;
		}
		break;

	default:
		return false;
	}

	return true;
}
bool MsgPackOStream::getValue(iterator_type& it, key_type& key, i16& value) {
	if (it >= data_.via.array.size) {
		return false;
	}

	msgobj value_obj;
	if (type_ == TYPE_OBJECT) {
		value_obj = data_.via.map.ptr[it].val;
		data_.via.map.ptr[it].key.convert(key);
	}
	else {
		value_obj = data_.via.array.ptr[it];
	}
	++it;

	switch(value_obj.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (i16)value_obj.via.i64;
		break;

	case object_type::STR:
		try {
			value = lexical_cast<i16>(value_obj.via.str.ptr);
		}
		catch(bad_lexical_cast&) {
			return false;
		}
		break;

	default:
		return false;
	}

	return true;
}

bool MsgPackOStream::getValue(const key_type& key, u16& value) {
	if (type_ != TYPE_OBJECT)
		return false;

	output_object_map_type::iterator it = object_.find(key);
	if (it == object_.end())
		return false;

	switch(it->second.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (u16)it->second.via.u64;
		break;

	case object_type::STR:
		try {
			value = lexical_cast<u16>(it->second.via.str.ptr);
		}
		catch(bad_lexical_cast&) {
			return false;
		}
		break;

	default:
		return false;
	}

	return true;
}
bool MsgPackOStream::getValue(iterator_type& it, key_type& key, u16& value) {
	if (it >= data_.via.array.size) {
		return false;
	}

	msgobj value_obj;
	if (type_ == TYPE_OBJECT) {
		value_obj = data_.via.map.ptr[it].val;
		data_.via.map.ptr[it].key.convert(key);
	}
	else {
		value_obj = data_.via.array.ptr[it];
	}
	++it;

	switch(value_obj.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (u16)value_obj.via.u64;
		break;

	case object_type::STR:
		try {
			value = lexical_cast<u16>(value_obj.via.str.ptr);
		}
		catch(bad_lexical_cast&) {
			return false;
		}
		break;

	default:
		return false;
	}

	return true;
}

bool MsgPackOStream::getValue(const key_type& key, i32& value) {
	if (type_ != TYPE_OBJECT)
		return false;

	output_object_map_type::iterator it = object_.find(key);
	if (it == object_.end())
		return false;

	switch(it->second.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (i32)it->second.via.i64;

		break;

	case object_type::STR:
		try {
			value = lexical_cast<i32>(it->second.via.str.ptr);
		}
		catch(bad_lexical_cast&) {
			return false;
		}
		break;

	default:
		return false;
	}

	return true;
}
bool MsgPackOStream::getValue(iterator_type& it, key_type& key, i32& value) {
	if (it >= data_.via.array.size) {
		return false;
	}

	msgobj value_obj;
	if (type_ == TYPE_OBJECT) {
		value_obj = data_.via.map.ptr[it].val;
		data_.via.map.ptr[it].key.convert(key);
	}
	else {
		value_obj = data_.via.array.ptr[it];
	}
	++it;

	switch(value_obj.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (i32)value_obj.via.i64;

		break;

	case object_type::STR:
		try {
			value = lexical_cast<i32>(value_obj.via.str.ptr);
		}
		catch(bad_lexical_cast&) {
			return false;
		}
		break;

	default:
		return false;
	}

	return true;
}

bool MsgPackOStream::getValue(const key_type& key, u32& value) {
	if (type_ != TYPE_OBJECT)
		return false;

	output_object_map_type::iterator it = object_.find(key);
	if (it == object_.end())
		return false;

	switch(it->second.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (u32)it->second.via.u64;
		break;

	case object_type::STR:
		try {
			value = lexical_cast<u32>(it->second.via.str.ptr);
		}
		catch(bad_lexical_cast&) {
			return false;
		}
		break;

	default:
		return false;
	}

	return true;
}
bool MsgPackOStream::getValue(iterator_type& it, key_type& key, u32& value) {
	if (it >= data_.via.array.size) {
		return false;
	}

	msgobj value_obj;
	if (type_ == TYPE_OBJECT) {
		value_obj = data_.via.map.ptr[it].val;
		data_.via.map.ptr[it].key.convert(key);
	}
	else {
		value_obj = data_.via.array.ptr[it];
	}
	++it;

	switch(value_obj.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (u32)value_obj.via.u64;
		break;

	case object_type::STR:
		try {
			value = lexical_cast<u32>(value_obj.via.str.ptr);
		}
		catch(bad_lexical_cast&) {
			return false;
		}
		break;

	default:
		return false;
	}

	return true;
}

bool MsgPackOStream::getValue(const key_type& key, i64& value) {
	if (type_ != TYPE_OBJECT)
		return false;

	output_object_map_type::iterator it = object_.find(key);
	if (it == object_.end())
		return false;

	switch(it->second.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (i64)it->second.via.i64;
		break;

	case object_type::STR:
		try {
			value = lexical_cast<i64>(it->second.via.str.ptr);
		}
		catch(bad_lexical_cast&) {
			return false;
		}
		break;

	default:
		return false;
	}

	return true;
}
bool MsgPackOStream::getValue(iterator_type& it, key_type& key, i64& value) {
	if (it >= data_.via.array.size) {
		return false;
	}

	msgobj value_obj;
	if (type_ == TYPE_OBJECT) {
		value_obj = data_.via.map.ptr[it].val;
		data_.via.map.ptr[it].key.convert(key);
	}
	else {
		value_obj = data_.via.array.ptr[it];
	}
	++it;

	switch(value_obj.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (i64)value_obj.via.i64;
		break;

	case object_type::STR:
		try {
			value = lexical_cast<i64>(value_obj.via.str.ptr);
		}
		catch(bad_lexical_cast&) {
			return false;
		}
		break;

	default:
		return false;
	}

	return true;
}

bool MsgPackOStream::getValue(const key_type& key, u64& value) {
	if (type_ != TYPE_OBJECT)
		return false;

	output_object_map_type::iterator it = object_.find(key);
	if (it == object_.end())
		return false;

	switch(it->second.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (u64)it->second.via.u64;
		break;

	case object_type::STR:
		try {
			value = lexical_cast<u64>(it->second.via.str.ptr);
		}
		catch(bad_lexical_cast&) {
			return false;
		}
		break;

	default:
		return false;
	}

	return true;
}
bool MsgPackOStream::getValue(iterator_type& it, key_type& key, u64& value) {
	if (it >= data_.via.array.size) {
		return false;
	}

	msgobj value_obj;
	if (type_ == TYPE_OBJECT) {
		value_obj = data_.via.map.ptr[it].val;
		data_.via.map.ptr[it].key.convert(key);
	}
	else {
		value_obj = data_.via.array.ptr[it];
	}
	++it;

	switch(value_obj.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (u64)value_obj.via.u64;
		break;

	case object_type::STR:
		try {
			value = lexical_cast<u64>(value_obj.via.str.ptr);
		}
		catch(bad_lexical_cast&) {
			return false;
		}
		break;

	default:
		return false;
	}

	return true;
}

bool MsgPackOStream::getValue(const key_type& key, f32& value) {
	if (type_ != TYPE_OBJECT)
		return false;

	output_object_map_type::iterator it = object_.find(key);
	if (it == object_.end())
		return false;

	switch(it->second.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (f32)it->second.via.f64;
		break;

	case object_type::STR:
		try{
			value = lexical_cast<f32>(it->second.via.str.ptr);
		}
		catch(bad_lexical_cast&) {
			return false;
		}
		break;

	default:
		return false;
	}

	return true;
}
bool MsgPackOStream::getValue(iterator_type& it, key_type& key, f32& value) {
	if (it >= data_.via.array.size) {
		return false;
	}

	msgobj value_obj;
	if (type_ == TYPE_OBJECT) {
		value_obj = data_.via.map.ptr[it].val;
		data_.via.map.ptr[it].key.convert(key);
	}
	else {
		value_obj = data_.via.array.ptr[it];
	}
	++it;

	switch(value_obj.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (f32)value_obj.via.f64;
		break;

	case object_type::STR:
		try{
			value = lexical_cast<f32>(value_obj.via.str.ptr);
		}
		catch(bad_lexical_cast&) {
			return false;
		}
		break;

	default:
		return false;
	}

	return true;
}

bool MsgPackOStream::getValue(const key_type& key, f64& value) {
	if (type_ != TYPE_OBJECT)
		return false;

	output_object_map_type::iterator it = object_.find(key);
	if (it == object_.end())
		return false;

	switch(it->second.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (f64)it->second.via.f64;
		break;

	case object_type::STR:
		try {
			value = lexical_cast<f64>(it->second.via.str.ptr);
		}
		catch(bad_lexical_cast&) {
			return false;
		}
		break;

	default:
		return false;
	}

	return true;
}
bool MsgPackOStream::getValue(iterator_type& it, key_type& key, f64& value) {
	if (it >= data_.via.array.size) {
		return false;
	}

	msgobj value_obj;
	if (type_ == TYPE_OBJECT) {
		value_obj = data_.via.map.ptr[it].val;
		data_.via.map.ptr[it].key.convert(key);
	}
	else {
		value_obj = data_.via.array.ptr[it];
	}
	++it;

	switch(value_obj.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = (f64)value_obj.via.f64;
		break;

	case object_type::STR:
		value = lexical_cast<f64>(value_obj.via.str.ptr);
		break;

	default:
		return false;
	}

	return true;
}

bool MsgPackOStream::getValue(const key_type& key, bool& value) {
	if (type_ != TYPE_OBJECT)
		return false;

	output_object_map_type::iterator it = object_.find(key);
	if (it == object_.end())
		return false;

	switch(it->second.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = it->second.via.boolean;
		break;

	case object_type::STR:
		value = it->second.via.str.size > 0;
		break;

	default:
		return false;
	}

	return true;
}
bool MsgPackOStream::getValue(iterator_type& it, key_type& key, bool& value) {
	if (it >= data_.via.array.size) {
		return false;
	}

	msgobj value_obj;
	if (type_ == TYPE_OBJECT) {
		value_obj = data_.via.map.ptr[it].val;
		data_.via.map.ptr[it].key.convert(key);
	}
	else {
		value_obj = data_.via.array.ptr[it];
	}
	++it;

	switch(value_obj.type) {
	case object_type::BOOLEAN:
	case object_type::POSITIVE_INTEGER:
	case object_type::NEGATIVE_INTEGER:
	case object_type::FLOAT:
		value = value_obj.via.boolean;
		break;

	case object_type::STR:
		value = value_obj.via.str.size > 0;
		break;

	default:
		return false;
	}

	return true;
}

bool MsgPackOStream::getValue(const key_type& key, string& value) {
	if (type_ != TYPE_OBJECT)
		return false;

	output_object_map_type::iterator it = object_.find(key);
	if (it == object_.end())
		return false;

	switch(it->second.type) {
	case object_type::BOOLEAN:
		value = it->second.via.boolean? "true": "false";
		break;
	case object_type::POSITIVE_INTEGER:
		value = lexical_cast<string>(it->second.via.u64);
		break;
	case object_type::NEGATIVE_INTEGER:
		value = lexical_cast<string>(it->second.via.i64);
		break;
	case object_type::FLOAT:
		value = lexical_cast<string>(it->second.via.f64);
		break;

	case object_type::STR:
	case object_type::BIN:
		value.assign(it->second.via.str.ptr, it->second.via.str.size);
		break;

	default:
		return false;
	}

	return true;
}
bool MsgPackOStream::getValue(iterator_type& it, key_type& key, string& value) {
	if (it >= data_.via.array.size) {
		return false;
	}

	msgobj value_obj;
	if (type_ == TYPE_OBJECT) {
		value_obj = data_.via.map.ptr[it].val;
		data_.via.map.ptr[it].key.convert(key);
	}
	else {
		value_obj = data_.via.array.ptr[it];
	}
	++it;

	switch(value_obj.type) {
	case object_type::BOOLEAN:
		value = value_obj.via.boolean? "true": "false";
		break;
	case object_type::POSITIVE_INTEGER:
		value = lexical_cast<string>(value_obj.via.u64);
		break;
	case object_type::NEGATIVE_INTEGER:
		value = lexical_cast<string>(value_obj.via.i64);
		break;
	case object_type::FLOAT:
		value = lexical_cast<string>(value_obj.via.f64);
		break;

	case object_type::STR:
	case object_type::BIN:
		value.assign(value_obj.via.str.ptr, value_obj.via.str.size);
		break;

	default:
		return false;
	}

	return true;
}

bool MsgPackOStream::getValue(const key_type& key, msgobj& value) {
	if (type_ != TYPE_OBJECT)
		return false;

	output_object_map_type::iterator it = object_.find(key);
	if (it == object_.end())
		return false;

	switch(it->second.type) {
	case object_type::ARRAY:
	case object_type::MAP:
		value = it->second;
		break;

	default:
		return false;
	}

	return true;
}
bool MsgPackOStream::getValue(iterator_type& it, key_type& key, msgobj& value) {
	if (it >= data_.via.array.size) {
		return false;
	}

	if (type_ == TYPE_OBJECT) {
		value = data_.via.map.ptr[it].val;
		data_.via.map.ptr[it].key.convert(key);
	}
	else {
		value = data_.via.array.ptr[it];
	}
	++it;

	switch(value.type) {
	case object_type::ARRAY:
	case object_type::MAP:
		break;

	default:
		return false;
	}

	return true;
}

}}}
