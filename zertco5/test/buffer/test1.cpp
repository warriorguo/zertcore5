/*
 * test1.cpp
 *
 *  Created on: 2015年1月29日
 *      Author: Administrator
 */

#include <utils/buffer/SharedBuffer.h>

using namespace zertcore;
using namespace zertcore::utils;

void f1(const SharedBuffer& buff) {
	ZC_ASSERT(buff.size() == 60);
	for (u32 i = 0; i < 17; ++i) {
		::printf("%c", buff[i]);
	}
	::printf("\n");
}

void f2(const SharedBuffer& buff) {
	for (u32 i = 0; i < 17; ++i) {
		::printf("%c", buff[i]);
	}
	::printf("\n");
	f1(buff.slice(4));
}

void f3(SharedBuffer buff) {
	for (u32 i = 0; i < 17; ++i) {
		::printf("%c", buff[i]);
	}
	::printf("\n");

	f2(buff);
}

void f4() {
//	SharedBuffer buff_error_here;
	SharedBuffer buff(64);
	strcpy((char *)&buff[0], "show me the money");

	f3(buff);
}

int main() {
	f4();

	printf("the buffer should be released\n");

	return 0;
}

