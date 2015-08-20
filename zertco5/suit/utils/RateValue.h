/*
 * RateValue.h
 *
 *  Created on: 2015年7月14日
 *      Author: Administrator
 */

#ifndef ZERTCORE_SUIT_UTILS_RATEVALUE_H_
#define ZERTCORE_SUIT_UTILS_RATEVALUE_H_

#include <pch.h>
#include <utils/types.h>

namespace zertcore { namespace suit {

/**
 * RateValue
 */
struct RateValue : Unserializable<RateValue>, Serializable<RateValue>
{
	double						rate{0};
	double						value{0};

public:
	template <class Archiver>
	void serialize(Archiver& archiver) const {
		archiver["rate"] & rate;
		archiver["value"] & value;
	}

	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		archiver["rate"] & rate;
		archiver["value"] & value;

		return rate > 0 || value > 0;
	}

public:
	RateValue& operator += (const RateValue& rv);
	double operator() (const double& v) const;
};


RateValue operator+ (const RateValue& r1, const RateValue& r2);
double operator* (const RateValue& rv, const double& v);
double operator* (const double& v, const RateValue& rv);

}}


#endif /* SUIT_UTILS_RATEVALUE_H_ */
