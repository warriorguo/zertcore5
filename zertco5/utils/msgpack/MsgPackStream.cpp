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

MsgPackIStream::MsgPackIStream() : type_(TYPE_NONE), buffer_size_(0) {
	zone_ = zone_ptr(new msgzone());
	obj_.via.str.size = 0;
}
MsgPackIStream::MsgPackIStream(MsgPackIStream& stream) :
	type_(TYPE_NONE), zone_(stream.zone_), buffer_size_(0) {
	obj_.via.str.size = 0;
}

void MsgPackIStream::addList(const msgobj& val) {
	if (obj_.via.array.size >= buffer_size_) {
		setupBuffer(buffer_size_ + OBJECT_INIT_SIZE);
	}

	msgobj& v = obj_.via.array.ptr[obj_.via.map.size];
	v << val;

	obj_.via.array.size++;
}

void MsgPackIStream::addObject(const key_type& key, const msgobj& val) {
	if (obj_.via.map.size >= buffer_size_) {
		setupBuffer(buffer_size_ + OBJECT_INIT_SIZE);
	}

	msgobj& k = obj_.via.map.ptr[obj_.via.map.size].key;
	msgobj& v = obj_.via.map.ptr[obj_.via.map.size].val;
	new (&k) msgobj(key, *zone_);
	v << val;

	obj_.via.map.size++;
}

void MsgPackIStream::combine(const MsgPackIStream& stream) {
	for (u32 i = 0; i < stream.obj_.via.map.size; ++i) {
		addObject(stream.obj_.via.map.ptr[i].key.as<key_type>(), stream.obj_.via.map.ptr[i].val);
	}
}

msgobj MsgPackIStream::data() const {
	return obj_;
}

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
	unpacked_ = unpacked_ptr(new unpacked);
	try {
		unpack(*unpacked_, (const char *)source.data(), source.size());
	}
	catch(...) {
		ZCLOG(ERROR) << "Unpack failed" << End;
		return false;
	}

	data_ = unpacked_->get();
	return initData();;
}

bool MsgPackOStream::data(const msgobj& d) {
	data_ = d;
	return initData();
}

bool MsgPackOStream::initData() {
	if (data_.type == object_type::MAP) {
		data_.convert(object_);
		type_ = TYPE_OBJECT;
	}
	else if (data_.type == object_type::ARRAY) {
//		data_.convert(array_);
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

bool MsgPackOStream::getValue(const key_type& key, MsgPackOStream& value) {
	if (type_ != TYPE_OBJECT)
		return false;

	output_object_map_type::iterator it = object_.find(key);
	if (it == object_.end())
		return false;

	switch(it->second.type) {
	case object_type::ARRAY:
	case object_type::MAP:
		value.data_ = it->second;
		value.unpacked_ = unpacked_;
		break;

	default:
		return false;
	}

	return value.initData();
}
bool MsgPackOStream::getValue(iterator_type& it, key_type& key, MsgPackOStream& value) {
	if (it >= data_.via.array.size) {
		return false;
	}

	if (type_ == TYPE_OBJECT) {
		data_.via.map.ptr[it].key.convert(key);
		value.data_ = data_.via.map.ptr[it].val;
	}
	else {
		value.data_ = data_.via.array.ptr[it];
	}
	++it;

	switch(value.data_.type) {
	case object_type::ARRAY:
	case object_type::MAP:
		value.unpacked_ = unpacked_;
		break;

	default:
		return false;
	}

	return value.initData();
}

value_type MsgPackOStream::getType(const key_type& key) const {
	if (type_ != TYPE_OBJECT)
		return TYPE_NONE;

	output_object_map_type::const_iterator it = object_.find(key);
	if (it == object_.end())
		return TYPE_NONE;

	switch(it->second.type) {
	case object_type::ARRAY:
		return TYPE_ARRAY;
	case object_type::MAP:
		return TYPE_OBJECT;
	case object_type::NEGATIVE_INTEGER:
		return TYPE_I64;
	case object_type::POSITIVE_INTEGER:
		return TYPE_U64;
	case object_type::FLOAT:
		return TYPE_DOUBLE;
	case object_type::STR:
		return TYPE_STRING;
	default:
		break;
	}

	return TYPE_NONE;
}

value_type MsgPackOStream::getType(const iterator_type& it) const {
	if (it >= data_.via.array.size) {
		return false;
	}

	msgobj value_obj;
	if (type_ == TYPE_OBJECT) {
		value_obj = data_.via.map.ptr[it].val;
	}
	else {
		value_obj = data_.via.array.ptr[it];
	}

	switch(value_obj.type) {
	case object_type::ARRAY:
		return TYPE_ARRAY;
	case object_type::MAP:
		return TYPE_OBJECT;
	case object_type::NEGATIVE_INTEGER:
		return TYPE_I64;
	case object_type::POSITIVE_INTEGER:
		return TYPE_U64;
	case object_type::FLOAT:
		return TYPE_DOUBLE;
	case object_type::STR:
		return TYPE_STRING;
	default:
		break;
	}

	return TYPE_NONE;
}

}}}
