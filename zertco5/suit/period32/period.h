/*
 * period.h
 *
 *  Created on: 2015年9月15日
 *      Author: Administrator
 */

#ifndef ZERTCORE_SUIT_PERIOD32_PERIOD_H_
#define ZERTCORE_SUIT_PERIOD32_PERIOD_H_

#include <pch.h>
#include <utils/types.h>

namespace zertcore { namespace suit {

/**
 * the structure was design for 3 digit accuracy time and took just 32bit.
 * Mainly for transfer the time.
 * Notice the valid period was about 1 YEAR.
 */
struct Period32
{
	u32							value;

	Period32& now();

public:
	static void init();

private:
	static u32					offset_;
};

}}


#endif /* SUIT_TIME32_TIME_H_ */
