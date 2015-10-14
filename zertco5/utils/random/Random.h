/*
 * Random.h
 *
 *  Created on: 2015年7月6日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_RANDOM_RANDOM_H_
#define ZERTCORE_UTILS_RANDOM_RANDOM_H_

#include <pch.h>
#include <utils/types.h>
#include <utils/group/Group.h>

namespace zertcore{ namespace utils {

/**
 * Random
 */
struct Random
{
	static void init();

	static double value();
	/**
	 * rate >= 1: 100% happen!
	 * rate <= 0: 100% not happen
	 * 0 < rate < 1: (rate * 100) percent happen
	 */
	static bool happen(double rate);

	template <typename Iterator>
	static Iterator get(Iterator begin, Iterator end, size_t size = 0) {
		size_t r = size;
		if (r == 0)
			for (Iterator it = begin; it != end; ++it) ++r;

		r = r * value();
		Iterator it = begin;
		for (; r && it != end; ++it, --r);

		return it;
	}

private:
	static i32					seed_;
};

}}


#endif /* UTILS_RANDOM_RANDOM_H_ */
