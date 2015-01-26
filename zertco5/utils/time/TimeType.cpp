/*
 * TimeType.cpp
 *
 *  Created on: 2015Äê1ÔÂ3ÈÕ
 *      Author: Administrator
 */

#include <pch.h>

#include "TimeType.h"


namespace zertcore{ namespace time{

TimeType operator + (const TimeType& t1, const TimeType& t2) {
	TimeType r(t1); r += t2;
	return r;
}
TimeType operator - (const TimeType& t1, const TimeType& t2) {
	TimeType r(t1); r -= t2;
	return r;
}

}}
