/*
 * test1.cpp
 *
 *  Created on: 2015��1��22��
 *      Author: Administrator
 */

#include <object/PoolObject.h>
#include <core/Runtime.h>
#include <log/Log.h>

using namespace zertcore;
using namespace zertcore::object;
using namespace zertcore::core;

struct Test : public PoolObject<Test>
{
	uint a, b, c;

	ZC_TO_STRING("a" << a << "b" << b << "c" << c);
};

int main() {
	RT.init([]() {
		ZCLOG(NOTICE) << "Hello there!" << End;
		Test::ptr test = Test::create();
		test->a = 1;
		test->b = 2;
		test->c = 3;

		ZCLOG(ERROR) << test << End;
	});
	RT.run();

	return 0;
}
