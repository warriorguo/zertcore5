/*
 * TimeType.h
 *
 *  Created on: 2014-10-3
 *      Author: Administrator
 */

#ifndef ZERTCORE_TIMETYPE_H_
#define ZERTCORE_TIMETYPE_H_

#include <pch.h>
#include <utils/types.h>

#include <object/ObjectBase.h>

#include <serialize/Serialize.h>
#include <serialize/Unserialize.h>

namespace zertcore{ namespace time{
using namespace zertcore::object;
using namespace zertcore::serialization;
}}

namespace zertcore{ namespace time{

/**
 * TimeType
 */
class TimeType :
		public ObjectBase<TimeType>,
		public Serializable<TimeType>,
		public Unserializable<TimeType>
{
public:
	typedef f64								type;

public:
	mutable type				value;

	ZC_TO_STRING("Time");

public:
	TimeType() : value(0) {}
	TimeType(const TimeType& tt) : value(tt.value) {}
	TimeType(const TimeType&& tt) : value(tt.value) {}
	TimeType(const type& t) : value(t) {}

public:
	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		return archiver("time") >> value;
	}
	template <class Archiver>
	Archiver& serialize(Archiver& archiver) {
		return archiver("time") << value;
	}

public:
	const TimeType& getTime() const {
		struct timeval tv;
		gettimeofday(&tv, NULL);

		value = (type)tv.tv_sec +
				(type)tv.tv_usec / 1000000.0;
		return *this;
	}

public:
	const TimeType& operator =(const TimeType& tt) {
		value = tt.value;
		return *this;
	}
	const TimeType& operator =(const type& t) {
		value = t;
		return *this;
	}

public:
	const TimeType& operator +=(const TimeType& tt) {
		value += tt.value;
		return *this;
	}
	const TimeType& operator +=(const type& t) {
		value += t;
		return *this;
	}

public:
	const TimeType& operator -=(const TimeType& tt) {
		value -= tt.value;
		return *this;
	}
	const TimeType& operator -=(const type& t) {
		value -= t;
		return *this;
	}

public:
	bool operator ==(const TimeType& tt) const {
		return value == tt.value;
	}
	bool operator !=(const TimeType& tt) const {
		return value != tt.value;
	}
	bool operator >(const TimeType& tt) const {
		return value > tt.value;
	}
	bool operator >=(const TimeType& tt) const {
		return value >= tt.value;
	}
	bool operator <(const TimeType& tt) const {
		return value < tt.value;
	}
	bool operator <=(const TimeType& tt) const {
		return value <= tt.value;
	}
};

TimeType operator + (const TimeType& t1, const TimeType& t2) {
	TimeType r(t1); r += t2;
	return r;
}
TimeType operator - (const TimeType& t1, const TimeType& t2) {
	TimeType r(t1); r -= t2;
	return r;
}

}}

namespace zertcore{
typedef time::TimeType						time_type;
}

#endif /* TIMETYPE_H_ */
