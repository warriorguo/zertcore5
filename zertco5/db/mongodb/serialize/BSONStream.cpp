/*
 * BSONStream.cpp
 *
 *  Created on: 2015��1��19��
 *      Author: Administrator
 */

#include "BSONStream.h"

namespace zertcore { namespace db { namespace mongodb { namespace serialization {

bool BSONOStream::getValue(const key_type& key, i8& value) {
	BSONElement elemt = data_.getField(key);

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
bool BSONOStream::getValue(iterator_type& it, key_type& key, string& value) {
	if (!it.more())
		return false;

	BSONElement elemt = it.next();
	key = elemt.fieldName();

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

bool BSONOStream::getValue(const key_type& key, BSONObj& stream) {
	BSONElement elemt = data_.getField(key);

	if (elemt.eoo()) {
		return false;
	}
	if (elemt.type() != BSONType::Object && elemt.type() != BSONType::Array) {
		return false;
	}

	stream = elemt.Obj();
	return true;
}
bool BSONOStream::getValue(iterator_type& it, key_type& key, BSONObj& stream) {
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

	stream = elemt.Obj();
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
}

}}}}
