/*
 * TimeType.cpp
 *
 *  Created on: 2015��1��3��
 *      Author: Administrator
 */

#include <pch.h>

#include "TimeType.h"


namespace zertcore{ namespace time_utils {

TimeType operator + (const TimeType& t1, const TimeType& t2) {
	TimeType r(t1); r += t2;
	return r;
}
TimeType operator - (const TimeType& t1, const TimeType& t2) {
	TimeType r(t1); r -= t2;
	return r;
}

}}
