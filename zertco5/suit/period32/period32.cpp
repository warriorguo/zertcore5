/*
 * period32.cpp
 *
 *  Created on: 2015年9月15日
 *      Author: Administrator
 */

#include "period.h"

namespace zertcore { namespace suit {
u32								Period32::offset_ = 0;
}}

namespace zertcore { namespace suit {

Period32& Period32::
now() {
	struct timeval tv;
	gettimeofday(&tv, NULL);

	value = ((tv.tv_sec - offset_) * 1000) + (tv.tv_usec / 1000);

	return *this;
}

void Period32::
init() {
	offset_ = time(NULL);
}

}}
