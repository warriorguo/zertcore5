/*
 * BSONStream.cpp
 *
 *  Created on: 2015��1��19��
 *      Author: Administrator
 */

#include "BSONStream.h"

namespace zertcore { namespace db { namespace mongodb { namespace serialization {

void BSONIStream::addObject(const key_type& key, const u64& v) {
	if (label_.l_) {
		obj_ << key << label_ << (long long)v;
		label_.l_ = NULL;
	}
	else
		obj_ << key << (long long)v;
}

void BSONIStream::addObject(const key_type& key, const SharedBuffer& v) {
	if (label_.l_) {
		ZCLOG(FATAL) << "Cant take SharedBuffer to compute" << End;
	}
	else
		obj_.appendBinData(key, v.size(), BinDataGeneral, v.data());
}

const BSONObj& BSONIStream::data() const {
	if (!result_.isEmpty())
		return result_;

	if (obj_.len()) {
		return result_ = obj_.obj();
	}

	return result_;
}

BSONObj& BSONIStream::data() {
	if (!result_.isEmpty())
		return result_;

	if (obj_.len()) {
		return result_ = obj_.obj();
	}

	return result_;
}

}}}}

namespace zertcore { namespace db { namespace mongodb { namespace serialization {

bool BSONOStream::getValue(const key_type& key, i8& value) {
	BSONElement elemt = data_.getField(key);
	if (elemt.eoo())
		return false;

	if (elemt.type() != NumberInt) {
		try {
			switch (elemt.type()) {
			case NumberDouble:
				value = (i8)elemt.Double();
				return true;
			case String:
				value = lexical_cast<i8>(elemt.String());
				return true;
			case NumberLong:
				value = i8(elemt.Long());
				return true;
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}
		return false;
	}

	value = (i8)elemt.Int();
	return true;
}
bool BSONOStream::getValue(iterator_type& it, key_type& key, i8& value) {
	if (!it.more())
		return false;

	BSONElement elemt = it.next();
	key = elemt.fieldName();

	if (elemt.type() != NumberInt) {
		try {
			switch (elemt.type()) {
			case NumberDouble:
				value = (i8)elemt.Double();
				return true;
			case String:
				value = lexical_cast<i8>(elemt.String());
				return true;
			case NumberLong:
				value = i8(elemt.Long());
				return true;
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}
		return false;
	}

	value = (i8)elemt.Int();
	return true;
}

bool BSONOStream::getValue(const key_type& key, i16& value) {
	BSONElement elemt = data_.getField(key);
	if (elemt.eoo())
		return false;

	if (elemt.type() != NumberInt) {
		try {
			switch (elemt.type()) {
			case NumberDouble:
				value = (i16)elemt.Double();
				return true;
			case String:
				value = lexical_cast<i16>(elemt.String());
				return true;
			case NumberLong:
				value = i16(elemt.Long());
				return true;
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}
		return false;
	}

	value = (i16)elemt.Int();
	return true;
}
bool BSONOStream::getValue(iterator_type& it, key_type& key, i16& value) {
	if (!it.more())
		return false;

	BSONElement elemt = it.next();
	key = elemt.fieldName();

	if (elemt.type() != NumberInt) {
		try {
			switch (elemt.type()) {
			case NumberDouble:
				value = (i16)elemt.Double();
				return true;
			case String:
				value = lexical_cast<i16>(elemt.String());
				return true;
			case NumberLong:
				value = i16(elemt.Long());
				return true;
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}
		return false;
	}

	value = (i16)elemt.Int();
	return true;
}

bool BSONOStream::getValue(const key_type& key, i32& value) {
	BSONElement elemt = data_.getField(key);
	if (elemt.eoo())
		return false;

	if (elemt.type() != NumberInt) {
		try {
			switch (elemt.type()) {
			case NumberDouble:
				value = (i32)elemt.Double();
				return true;
			case String:
				value = lexical_cast<i32>(elemt.String());
				return true;
			case NumberLong:
				value = i32(elemt.Long());
				return true;
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}
		return false;
	}

	value = (i32)elemt.Int();
	return true;
}
bool BSONOStream::getValue(iterator_type& it, key_type& key, i32& value) {
	if (!it.more())
		return false;

	BSONElement elemt = it.next();
	key = elemt.fieldName();

	if (elemt.type() != NumberInt) {
		try {
			switch (elemt.type()) {
			case NumberDouble:
				value = (i32)elemt.Double();
				return true;
			case String:
				value = lexical_cast<i32>(elemt.String());
				return true;
			case NumberLong:
				value = i32(elemt.Long());
				return true;
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}
		return false;
	}

	value = (i32)elemt.Int();
	return true;
}

bool BSONOStream::getValue(const key_type& key, i64& value) {
	BSONElement elemt = data_.getField(key);
	if (elemt.eoo())
		return false;

	if (elemt.type() != NumberLong) {
		try {
			switch (elemt.type()) {
			case NumberDouble:
				value = (i64)elemt.Double();
				return true;
			case String:
				value = lexical_cast<i64>(elemt.String());
				return true;
			case NumberInt:
				value = i64(elemt.Int());
				return true;
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}
		return false;
	}

	value = (i64)elemt.Long();
	return true;
}
bool BSONOStream::getValue(iterator_type& it, key_type& key, i64& value) {
	if (!it.more())
		return false;

	BSONElement elemt = it.next();
	key = elemt.fieldName();

	if (elemt.type() != NumberLong) {
		try {
			switch (elemt.type()) {
			case NumberDouble:
				value = (i64)elemt.Double();
				return true;
			case String:
				value = lexical_cast<i64>(elemt.String());
				return true;
			case NumberInt:
				value = i64(elemt.Int());
				return true;
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}
		return false;
	}

	value = (i64)elemt.Long();
	return true;
}

bool BSONOStream::getValue(const key_type& key, u8& value) {
	BSONElement elemt = data_.getField(key);
	if (elemt.eoo())
		return false;

	if (elemt.type() != NumberInt) {
		try {
			switch (elemt.type()) {
			case NumberDouble:
				value = (u8)elemt.Double();
				return true;
			case String:
				value = lexical_cast<u8>(elemt.String());
				return true;
			case NumberLong:
				value = u8(elemt.Long());
				return true;
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}
		return false;
	}

	value = (u8)elemt.Int();
	return true;
}
bool BSONOStream::getValue(iterator_type& it, key_type& key, u8& value) {
	if (!it.more())
		return false;

	BSONElement elemt = it.next();
	key = elemt.fieldName();

	if (elemt.type() != NumberInt) {
		try {
			switch (elemt.type()) {
			case NumberDouble:
				value = (u8)elemt.Double();
				return true;
			case String:
				value = lexical_cast<u8>(elemt.String());
				return true;
			case NumberLong:
				value = u8(elemt.Long());
				return true;
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}
		return false;
	}

	value = (u8)elemt.Int();
	return true;
}

bool BSONOStream::getValue(const key_type& key, u16& value) {
	BSONElement elemt = data_.getField(key);
	if (elemt.eoo())
		return false;

	if (elemt.type() != NumberInt) {
		try {
			switch (elemt.type()) {
			case NumberDouble:
				value = (u16)elemt.Double();
				return true;
			case String:
				value = lexical_cast<u16>(elemt.String());
				return true;
			case NumberLong:
				value = u16(elemt.Long());
				return true;
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}
		return false;
	}

	value = (u16)elemt.Int();
	return true;
}
bool BSONOStream::getValue(iterator_type& it, key_type& key, u16& value) {
	if (!it.more())
		return false;

	BSONElement elemt = it.next();
	key = elemt.fieldName();

	if (elemt.type() != NumberInt) {
		try {
			switch (elemt.type()) {
			case NumberDouble:
				value = (u16)elemt.Double();
				return true;
			case String:
				value = lexical_cast<u16>(elemt.String());
				return true;
			case NumberLong:
				value = u16(elemt.Long());
				return true;
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}
		return false;
	}

	value = (u16)elemt.Int();
	return true;
}

bool BSONOStream::getValue(const key_type& key, u32& value) {
	BSONElement elemt = data_.getField(key);
	if (elemt.eoo())
		return false;

	if (elemt.type() != NumberInt) {
		try {
			switch (elemt.type()) {
			case NumberDouble:
				value = (u32)elemt.Double();
				return true;
			case String:
				value = lexical_cast<u32>(elemt.String());
				return true;
			case NumberLong:
				value = u32(elemt.Long());
				return true;
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}
		return false;
	}

	value = (u32)elemt.Int();
	return true;
}
bool BSONOStream::getValue(iterator_type& it, key_type& key, u32& value) {
	if (!it.more())
		return false;

	BSONElement elemt = it.next();
	key = elemt.fieldName();

	if (elemt.type() != NumberInt) {
		try {
			switch (elemt.type()) {
			case NumberDouble:
				value = (u32)elemt.Double();
				return true;
			case String:
				value = lexical_cast<u32>(elemt.String());
				return true;
			case NumberLong:
				value = u32(elemt.Long());
				return true;
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}
		return false;
	}

	value = (u32)elemt.Int();
	return true;
}

bool BSONOStream::getValue(const key_type& key, u64& value) {
	BSONElement elemt = data_.getField(key);
	if (elemt.eoo())
		return false;

	if (elemt.type() != NumberLong) {
		try {
			switch (elemt.type()) {
			case NumberDouble:
				value = (u64)elemt.Double();
				return true;
			case String:
				value = lexical_cast<u64>(elemt.String());
				return true;
			case NumberInt:
				value = u64(elemt.Int());
				return true;
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}
		return false;
	}

	value = (u64)elemt.Long();
	return true;
}
bool BSONOStream::getValue(iterator_type& it, key_type& key, u64& value) {
	if (!it.more())
		return false;

	BSONElement elemt = it.next();
	key = elemt.fieldName();

	if (elemt.type() != NumberLong) {
		try {
			switch (elemt.type()) {
			case NumberDouble:
				value = (u64)elemt.Double();
				return true;
			case String:
				value = lexical_cast<u64>(elemt.String());
				return true;
			case NumberInt:
				value = u64(elemt.Int());
				return true;
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}
		return false;
	}

	value = (u64)elemt.Long();
	return true;
}

bool BSONOStream::getValue(const key_type& key, f32& value) {
	BSONElement elemt = data_.getField(key);
	if (elemt.eoo())
		return false;

	if (elemt.type() != NumberDouble) {
		try {
			switch (elemt.type()) {
			case NumberLong:
				value = (f32)elemt.Long();
				return true;
			case String:
				value = lexical_cast<f32>(elemt.String());
				return true;
			case NumberInt:
				value = f32(elemt.Int());
				return true;
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}
		return false;
	}

	value = (f32)elemt.Double();
	return true;
}
bool BSONOStream::getValue(iterator_type& it, key_type& key, f32& value) {
	if (!it.more())
		return false;

	BSONElement elemt = it.next();
	key = elemt.fieldName();

	if (elemt.type() != NumberDouble) {
		try {
			switch (elemt.type()) {
			case NumberLong:
				value = (f32)elemt.Long();
				return true;
			case String:
				value = lexical_cast<f32>(elemt.String());
				return true;
			case NumberInt:
				value = f32(elemt.Int());
				return true;
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}
		return false;
	}

	value = (f32)elemt.Double();
	return true;
}

bool BSONOStream::getValue(const key_type& key, f64& value) {
	BSONElement elemt = data_.getField(key);
	if (elemt.eoo())
		return false;

	if (elemt.type() != NumberDouble) {
		try {
			switch (elemt.type()) {
			case NumberLong:
				value = (f64)elemt.Long();
				return true;
			case String:
				value = lexical_cast<f64>(elemt.String());
				return true;
			case NumberInt:
				value = f64(elemt.Int());
				return true;
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}
		return false;
	}

	value = (f64)elemt.Double();
	return true;
}
bool BSONOStream::getValue(iterator_type& it, key_type& key, f64& value) {
	if (!it.more())
		return false;

	BSONElement elemt = it.next();
	key = elemt.fieldName();

	if (elemt.type() != NumberDouble) {
		try {
			switch (elemt.type()) {
			case NumberLong:
				value = (f64)elemt.Long();
				return true;
			case String:
				value = lexical_cast<f64>(elemt.String());
				return true;
			case NumberInt:
				value = f64(elemt.Int());
				return true;
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}
		return false;
	}

	value = (f64)elemt.Double();
	return true;
}

bool BSONOStream::getValue(const key_type& key, bool& value) {
	BSONElement elemt = data_.getField(key);
	if (elemt.eoo())
		return false;

	if (elemt.type() != Bool) {
		try {
			switch (elemt.type()) {
			case NumberLong:
				value = elemt.Long() != 0;
				return true;
			case NumberInt:
				value = elemt.Int() != 0;
				return true;
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}
		return false;
	}

	value = elemt.Bool();
	return true;
}
bool BSONOStream::getValue(iterator_type& it, key_type& key, bool& value) {
	if (!it.more())
		return false;

	BSONElement elemt = it.next();
	key = elemt.fieldName();

	if (elemt.type() != Bool) {
		try {
			switch (elemt.type()) {
			case NumberLong:
				value = elemt.Long() != 0;
				return true;
			case NumberInt:
				value = elemt.Int() != 0;
				return true;
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}
		return false;
	}

	value = elemt.Bool();
	return true;
}

bool BSONOStream::getValue(const key_type& key, string& value) {
	BSONElement elemt = data_.getField(key);
	if (elemt.eoo())
		return false;

	if (elemt.type() != String) {
		try {
			switch (elemt.type()) {
			case Timestamp:
				value = elemt.Date().toString();
				return true;
			case NumberLong:
				value = lexical_cast<string>(elemt.Long());
				return true;
			case NumberInt:
				value = lexical_cast<string>(elemt.Int());
				return true;
			case NumberDouble:
				value = lexical_cast<string>(elemt.Double());
				return true;
			case Bool:
				value = lexical_cast<string>(elemt.Bool());
				return true;
			case BinData: {
				int len = 0;
				const char* d = elemt.binData(len);
				value.assign(d, len);
				return true;
			}
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}
		return false;
	}

	value = move(elemt.String());
	return true;
}
bool BSONOStream::getValue(iterator_type& it, key_type& key, string& value) {
	if (!it.more())
		return false;

	BSONElement elemt = it.next();
	key = elemt.fieldName();

	if (elemt.type() != String) {
		try {
			switch (elemt.type()) {
			case Timestamp:
				value = elemt.Date().toString();
				return true;
			case NumberLong:
				value = lexical_cast<string>(elemt.Long());
				return true;
			case NumberInt:
				value = lexical_cast<string>(elemt.Int());
				return true;
			case NumberDouble:
				value = lexical_cast<string>(elemt.Double());
				return true;
			case Bool:
				value = lexical_cast<string>(elemt.Bool());
				return true;
			case BinData: {
				int len = 0;
				const char* d = elemt.binData(len);
				value.assign(d, len);
				return true;
			}
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}
		return false;
	}

	value = move(elemt.String());
	return true;
}

bool BSONOStream::getValue(const key_type& key, SharedBuffer& value) {
	BSONElement elemt = data_.getField(key);
	if (elemt.eoo())
		return false;

	if (elemt.type() == BinData) {
		int len = 0;
		const char* d = elemt.binData(len);
		value.assign(d, len);
		return true;
	}
	else if (elemt.type() == String) {
		value.assign(elemt.valuestr(), elemt.valuestrsize());
		return true;
	}

	return false;
}
bool BSONOStream::getValue(iterator_type& it, key_type& key, SharedBuffer& value) {
	if (!it.more())
		return false;

	BSONElement elemt = it.next();
	key = elemt.fieldName();

	if (elemt.type() == BinData) {
		int len = 0;
		const char* d = elemt.binData(len);
		value.assign(d, len);
		return true;
	}
	else if (elemt.type() == String) {
		value.assign(elemt.valuestr(), elemt.valuestrsize());
		return true;
	}

	return false;
}

bool BSONOStream::getValue(const key_type& key, BSONOStream& stream) {
	BSONElement elemt = data_.getField(key);
	if (elemt.eoo())
		return false;

	if (elemt.type() != BSONType::Object && elemt.type() != BSONType::Array) {
		return false;
	}

	stream.data(elemt.Obj().getOwned());
	return true;
}
bool BSONOStream::getValue(iterator_type& it, key_type& key, BSONOStream& stream) {
	if (!it.more())
		return false;

	BSONElement elemt = it.next();
	key = elemt.fieldName();

	if (elemt.eoo()) {
		return false;
	}
	if (elemt.type() != BSONType::Object && elemt.type() != BSONType::Array) {
		return false;
	}

	stream.data(elemt.Obj().getOwned());
	return true;
}

value_type BSONOStream::getType(const key_type& key) const {
	BSONElement elemt = data_.getField(key);
	if (elemt.eoo())
		return TYPE_NONE;

	switch(elemt.type()) {
	case BSONType::Array:
		return TYPE_ARRAY;
	case BSONType::Object:
		return TYPE_OBJECT;
	case BSONType::NumberDouble:
		return TYPE_DOUBLE;
	case BSONType::NumberInt:
		return TYPE_I32;
	case BSONType::NumberLong:
		return TYPE_I64;
	case BSONType::String:
		return TYPE_STRING;

	default:
		break;
	}

	return TYPE_NONE;
}

value_type BSONOStream::getType(iterator_type it) const {
	BSONElement elemt = *it;
	if (elemt.eoo())
		return TYPE_NONE;

	switch(elemt.type()) {
	case BSONType::Array:
		return TYPE_ARRAY;
	case BSONType::Object:
		return TYPE_OBJECT;
	case BSONType::NumberDouble:
		return TYPE_DOUBLE;
	case BSONType::NumberInt:
		return TYPE_I32;
	case BSONType::NumberLong:
		return TYPE_I64;
	case BSONType::String:
		return TYPE_STRING;

	default:
		break;
	}

	return TYPE_NONE;
}

bool BSONOStream::buffer(const SharedBuffer& buf) {
	try {
		int size = (int)buf.size();
		data(fromjson((const char *)buf.data(), &size));
	}catch(MsgAssertionException&) {
		return false;
	}

	return true;
}

void BSONOStream::data(const BSONObj& d) {
	data_ = d;
}

}}}}
