/*
 * RateValue.cpp
 *
 *  Created on: 2015年7月14日
 *      Author: Administrator
 */

#include "RateValue.h"

namespace zertcore { namespace suit {

RateValue& RateValue::
operator += (const RateValue& rv) {
	rate += rv.rate;
	value += rv.value;
	return *this;
}

double RateValue::
operator() (const double& v) const {
	return (v + value) * (1 + rate);
}

RateValue operator+ (const RateValue& r1, const RateValue& r2) {
	RateValue rv(r1);
	return rv += r2;
}

double operator* (const RateValue& rv, const double& v) {
	return rv(v);
}

double operator* (const double& v, const RateValue& rv) {
	return rv(v);
}

}}

