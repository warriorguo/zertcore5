/*
 * BSONStream.cpp
 *
 *  Created on: 2015Äê1ÔÂ19ÈÕ
 *      Author: Administrator
 */

#include "BSONStream.h"

namespace zertcore { namespace db { namespace mongodb { namespace serialization {

void BSONIStream::setValue(const key_type& key, const char* v) {
	if (type_ == TYPE_OBJECT) {
		obj_.append(key, v);
	}
	else {
		array_.append(v);
	}
}

void BSONIStream::setStream(const key_type& key, BSONIStream& stream, const op_code_type& op_code) {
	if (!op_code) {
		if (type_ == TYPE_OBJECT)
			obj_.append(key, stream.data());
		else
			array_.append(stream.data());

		return ;
	}

	switch(op_code) {
	case OP_IN:
		ZC_ASSERT(type_ == TYPE_OBJECT);
		ZC_ASSERT(stream.type_ == TYPE_ARRAY);
		obj_ << key << BSON("$in" << stream.data());
		break;
	case OP_NIN:
		ZC_ASSERT(type_ == TYPE_OBJECT);
		ZC_ASSERT(stream.type_ == TYPE_ARRAY);
		obj_ << key << BSON("$nin" << stream.data());
		break;
	case OP_AND:
		ZC_ASSERT(type_ == TYPE_OBJECT);
		obj_.appendElements(stream.data());
		break;
	case OP_OR:
		ZC_ASSERT(type_ == TYPE_OBJECT);
		or_flag_ = true;
		array_.append(stream.data());
		break;

	default:
		ZCLOG(FINAL) << "Unsopprt OPCODE:" << (u32)op_code << End;
	}
}

string BSONIStream::str() const {
	return data().jsonString();
}

BSONObj BSONIStream::data() const {
	if (!result_.isEmpty())
		return result_;

	if (type_ == TYPE_OBJECT) {
		if (or_flag_) {
			obj_.append("$or", array_.obj());
		}

		return result_ = obj_.obj();
	}

	return result_ = array_.obj();
}

}}}}

namespace zertcore { namespace db { namespace mongodb { namespace serialization {

bool BSONOStream::getValue(key_type& key, i8& value) {
	BSONElement elemt;
	if (type_ == TYPE_ARRAY) {
		if (!iter_.more())
			return false;

		elemt = iter_.next();
		key = elemt.fieldName();
	}
	else {
		elemt = data_.getField(key);
	}

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
bool BSONOStream::getValue(key_type& key, i16& value) {
	BSONElement elemt;
	if (type_ == TYPE_ARRAY) {
		if (!iter_.more())
			return false;

		elemt = iter_.next();
		key = elemt.fieldName();
	}
	else {
		elemt = data_.getField(key);
	}

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
bool BSONOStream::getValue(key_type& key, i32& value) {
	BSONElement elemt;
	if (type_ == TYPE_ARRAY) {
		if (!iter_.more())
			return false;

		elemt = iter_.next();
		key = elemt.fieldName();
	}
	else {
		elemt = data_.getField(key);
	}

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
bool BSONOStream::getValue(key_type& key, i64& value) {
	BSONElement elemt;
	if (type_ == TYPE_ARRAY) {
		if (!iter_.more())
			return false;

		elemt = iter_.next();
		key = elemt.fieldName();
	}
	else {
		elemt = data_.getField(key);
	}

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

bool BSONOStream::getValue(key_type& key, u8& value) {
	BSONElement elemt;
	if (type_ == TYPE_ARRAY) {
		if (!iter_.more())
			return false;

		elemt = iter_.next();
		key = elemt.fieldName();
	}
	else {
		elemt = data_.getField(key);
	}

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
bool BSONOStream::getValue(key_type& key, u16& value) {
	BSONElement elemt;
	if (type_ == TYPE_ARRAY) {
		if (!iter_.more())
			return false;

		elemt = iter_.next();
		key = elemt.fieldName();
	}
	else {
		elemt = data_.getField(key);
	}

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
bool BSONOStream::getValue(key_type& key, u32& value) {
	BSONElement elemt;
	if (type_ == TYPE_ARRAY) {
		if (!iter_.more())
			return false;

		elemt = iter_.next();
		key = elemt.fieldName();
	}
	else {
		elemt = data_.getField(key);
	}

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
bool BSONOStream::getValue(key_type& key, u64& value) {
	BSONElement elemt;
	if (type_ == TYPE_ARRAY) {
		if (!iter_.more())
			return false;

		elemt = iter_.next();
		key = elemt.fieldName();
	}
	else {
		elemt = data_.getField(key);
	}

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

bool BSONOStream::getValue(key_type& key, f32& value) {
	BSONElement elemt;
	if (type_ == TYPE_ARRAY) {
		if (!iter_.more())
			return false;

		elemt = iter_.next();
		key = elemt.fieldName();
	}
	else {
		elemt = data_.getField(key);
	}

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
bool BSONOStream::getValue(key_type& key, f64& value) {
	BSONElement elemt;
	if (type_ == TYPE_ARRAY) {
		if (!iter_.more())
			return false;

		elemt = iter_.next();
		key = elemt.fieldName();
	}
	else {
		elemt = data_.getField(key);
	}

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

bool BSONOStream::getValue(key_type& key, bool& value) {
	BSONElement elemt;
	if (type_ == TYPE_ARRAY) {
		if (!iter_.more())
			return false;

		elemt = iter_.next();
		key = elemt.fieldName();
	}
	else {
		elemt = data_.getField(key);
	}

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

bool BSONOStream::getValue(key_type& key, string& value) {
	BSONElement elemt;
	if (type_ == TYPE_ARRAY) {
		if (!iter_.more())
			return false;

		elemt = iter_.next();
		key = elemt.fieldName();
	}
	else {
		elemt = data_.getField(key);
	}

	if (elemt.type() != String) {
		try {
			switch (elemt.type()) {
			case Timestamp:
				value = elemt.timestampTime().toString();
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
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}
		return false;
	}

	value = elemt.String();
	return true;
}

bool BSONOStream::getStream(key_type& key, BSONOStream& stream) {
	BSONElement elemt;
	if (type_ == TYPE_ARRAY) {
		if (!iter_.more()) {
			return false;
		}

		elemt = iter_.next();
		key = elemt.fieldName();
	}
	else {
		elemt = data_.getField(key);
	}

	if (elemt.eoo()) {
		return false;
	}
	if (elemt.type() != BSONType::Object && elemt.type() != BSONType::Array) {
		return false;
	}

	stream.data(elemt.Obj());
	return true;
}

bool BSONOStream::str(const string& source) {
	try {
		data(fromjson(source));
	}catch(MsgAssertionException&) {
		return false;
	}

	return true;
}

void BSONOStream::data(const BSONObj& d) {
	data_ = d;
	iter_ = data_.begin();
}

}}}}
