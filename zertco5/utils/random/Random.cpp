/*
 * Random.cpp
 *
 *  Created on: 2015年7月6日
 *      Author: Administrator
 */

#include "Random.h"

namespace zertcore { namespace utils {

i32 Random::seed_				= 0;
}}

namespace zertcore { namespace utils {

void Random::
init() {
	;
}

double Random::
value() {
	srand(seed_);
	seed_ = rand();
	return (double)seed_ / RAND_MAX;
}

bool Random::
happen(double rate) {
	return value() < rate;
}

}}
