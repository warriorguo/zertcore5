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

namespace zertcore{ namespace time_utils {
using namespace zertcore::object;
}}

namespace zertcore{ namespace time_utils {

/**
 * TimeType
 */
class TimeType :
		public ObjectBase<TimeType>
{
public:
	typedef f64								type;

public:
	mutable type				value;

	ZC_TO_STRING("value" << value);

public:
	TimeType() : value(0) {}
	TimeType(const TimeType& tt) : value(tt.value) {}
	TimeType(const TimeType&& tt) : value(tt.value) {}
	TimeType(const type& t) : value(t) {}
	TimeType(const __NOW& _) : value(0) {
		getTime();
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

/**
 * operators for time_type
 */
TimeType operator + (const TimeType& t1, const TimeType& t2);
TimeType operator - (const TimeType& t1, const TimeType& t2);

}}

namespace zertcore{
typedef time_utils::TimeType				time_type;
}

#endif /* TIMETYPE_H_ */
