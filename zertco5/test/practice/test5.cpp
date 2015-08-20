/*
 * test5.cpp
 *
 *  Created on: 2015年2月6日
 *      Author: Administrator
 */

#include <cstdio>

int main() {
	unsigned long long t = 0xECA16DB97F6789AC;
	t ^= 0x1357924680987654;
	t = ~t;

	unsigned int prestige_level = (t >> 48);
	t &= 0xFFFFFFFFFFFF;

	printf("prestige=%u %u\n", prestige_level, t);

	return 0;
}


