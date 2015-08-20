/*
 * BasicValues.h
 *
 *  Created on: 2015年6月27日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_CONDITION_BASICVALUES_H_
#define ZERTCORE_UTILS_CONDITION_BASICVALUES_H_

#include <pch.h>
#include <utils/types.h>

#include <serialize/config.h>

namespace zertcore { namespace utils {

#define EnableOPWithoutStr(op)		\
	bool operator op (const BasicValue& bv) const {\
		if (!isTypeComparable(bv))\
			return false;\
		switch(type) {\
		case TYPE_U32:\
			return u32_val op bv.u32_val;\
		case TYPE_I32:\
			return i32_val op bv.i32_val;\
		case TYPE_I64:\
			return i64_val op bv.i64_val;\
		case TYPE_U64:\
			return u64_val op bv.u64_val;\
		case TYPE_DOUBLE:\
			return double_val op bv.double_val;\
		default: break;\
		}\
		return false;\
	}
#define EnableOP(op)		\
	bool operator op (const BasicValue& bv) const {\
		if (!isTypeComparable(bv))\
			return false;\
		switch(type) {\
		case TYPE_U32:\
			return u32_val op bv.u32_val;\
		case TYPE_I32:\
			return i32_val op bv.i32_val;\
		case TYPE_I64:\
			return i64_val op bv.i64_val;\
		case TYPE_U64:\
			return u64_val op bv.u64_val;\
		case TYPE_DOUBLE:\
			return double_val op bv.double_val;\
		case TYPE_STRING:\
			return str_val op bv.str_val;\
		default: break;\
		}\
		return false;\
	}
/**
 * BasicValue
 */
struct BasicValue
{
	value_type					type;

	union {
		u32						u32_val;
		i32						i32_val;
		u64						u64_val;
		i64						i64_val;
		double					double_val;
	};
	string						str_val;

	BasicValue() : type(TYPE_NONE), u64_val(0) {}
	BasicValue(const u32& v) : type(TYPE_U32), u64_val(v) {}
	BasicValue(const i32& v) : type(TYPE_I32), i64_val(v) {}
	BasicValue(const u64& v) : type(TYPE_U64), u64_val(v) {}
	BasicValue(const i64& v) : type(TYPE_I64), i64_val(v) {}
	BasicValue(const double& v) : type(TYPE_DOUBLE), double_val(v) {}
	BasicValue(const string& v) : type(TYPE_STRING), str_val(v) {}

public:
/**
	template <class Archiver>
	void serialize(Archiver& archiver) const {
		archiver["type"] & type;
		switch(type) {
		case TYPE_U32:
			archiver["v"] & u32_val; return ;
		case TYPE_I32:
			archiver["v"] & i32_val; return ;
		case TYPE_I64:
			archiver["v"] & u64_val; return ;
		case TYPE_U64:
			archiver["v"] & i64_val; return ;
		case TYPE_DOUBLE:
			archiver["v"] & double_val; return ;
		case TYPE_STRING:
			archiver["v"] & str_val; return ;

		default:
			break;
		}
	}

	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		archiver["type"] & type;
		switch(type) {
		case TYPE_U32:
			return archiver["v"] & u32_val;
		case TYPE_I32:
			return archiver["v"] & i32_val;
		case TYPE_I64:
			return archiver["v"] & u64_val;
		case TYPE_U64:
			return archiver["v"] & i64_val;
		case TYPE_DOUBLE:
			return archiver["v"] & double_val;
		case TYPE_STRING:
			return archiver["v"] & str_val;

		default:
			break;
		}

		return false;
	}
*/
private:
	bool isTypeComparable(const BasicValue& bv) const {
		if (bv.type != type && type == TYPE_STRING)
			return false;
		return true;
	}

public:
	EnableOPWithoutStr(<)
	EnableOPWithoutStr(<=)
	EnableOPWithoutStr(>)
	EnableOPWithoutStr(>=)
	EnableOP(==)

	i64 operator% (const BasicValue& bv) const {
		if (!isTypeComparable(bv))
			return false;

		switch(type) {
		case TYPE_U32:
			return u32_val % bv.u32_val;
		case TYPE_I32:
			return i32_val % bv.i32_val;
		case TYPE_I64:
			return i64_val % bv.i64_val;
		case TYPE_U64:
			return u64_val % bv.u64_val;
		case TYPE_DOUBLE:
			return 1;
		case TYPE_STRING:
			return 1;

		default:
			break;
		}

		return 1;
	}

};

#undef EnableOP
}}

namespace boost {
using namespace zertcore;

template <>
struct hash<utils::BasicValue> :
	public std::unary_function<utils::BasicValue, size_t>
{
	size_t operator()(const utils::BasicValue& bv) const {
		switch(bv.type) {
		case TYPE_U32: return bv.u32_val;
		case TYPE_I32: return bv.i32_val;
		case TYPE_U64: return bv.u64_val;
		case TYPE_I64: return bv.i64_val;
		case TYPE_DOUBLE: {
			hash<double> hasher;
			return hasher(bv.double_val);
		}
		case TYPE_STRING: {
			hash<string> hasher;
			return hasher(bv.str_val);

		}
		default:
			break;
		}

		return 0;
	}
};

}

#include <serialize/Serializer_fwd.h>
#include <serialize/Unserializer_fwd.h>

namespace zertcore { namespace serialization {
using namespace zertcore::utils;

template <class Stream>
inline Serializer<Stream>& operator << (Serializer<Stream>& s, const BasicValue& v) {
	switch (v.type) {
	case TYPE_U32: {
		s.setValue(v.u32_val);
		break;
	}
	case TYPE_I32: {
		s.setValue(v.i32_val);
		break;
	}
	case TYPE_U64: {
		s.setValue(v.u64_val);
		break;
	}
	case TYPE_I64: {
		s.setValue(v.i64_val);
		break;
	}
	case TYPE_DOUBLE: {
		s.setValue(v.double_val);
		break;
	}
	case TYPE_STRING: {
		s.setValue(v.str_val);
		break;
	}
	default:
		break;
	}

	return s;
}

template <class Stream>
inline bool operator >> (const Unserializer<Stream>& s, BasicValue& v) {
	v.type = s.getElementType();

	switch (v.type) {
	case TYPE_U32: {
		return s.getValue(v.u64_val);
	}
	case TYPE_I32: {
		return s.getValue(v.i64_val);
	}
	case TYPE_U64: {
		return s.getValue(v.u64_val);
	}
	case TYPE_I64: {
		return s.getValue(v.i64_val);
	}
	case TYPE_DOUBLE: {
		return s.getValue(v.double_val);
	}
	case TYPE_STRING: {
		return s.getValue(v.str_val);
	}
	default:
		break;
	}

	return false;
}

}}


#endif /* UTILS_CONDITION_BASICVALUES_H_ */
