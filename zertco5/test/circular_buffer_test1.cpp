/*
 * circular_buffer_test1.cpp
 *
 *  Created on: 2015Äê1ÔÂ8ÈÕ
 *      Author: Administrator
 */

#include <pch.h>

using namespace zertcore;

#define PRINT 	do{for (auto it = b.begin(); it != b.end(); ++it) {::printf("%d ", *it);}::printf("\n");}while(false);

int main() {
	circular_buffer<int> b(10);

	b.push_back(1);
	b.push_back(2);
	b.push_back(3);
	b.push_back(4);
	b.push_back(5);
	b.push_back(6);
	b.push_back(7);
	b.push_back(8);
	b.push_back(9);
	b.push_back(10);

	PRINT;

	b.erase(b.begin(), b.begin() + 4);

	PRINT;

	b.push_back(11);
	b.push_back(12);
	b.push_back(13);
	b.push_back(14);
	b.push_back(15);
	b.push_back(16);

	int s[] = {11, 12, 13, 14};
	b.insert(b.end(), &s[0], &s[3]);

	PRINT;

	return 0;
}


