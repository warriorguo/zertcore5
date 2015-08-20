/*
 * serialize_object_test1.cpp
 *
 *  Created on: 2015年7月17日
 *      Author: Administrator
 */

#include <zertcore.h>

using namespace zertcore;
using namespace zertcore::utils;
using namespace zertcore::serialization;

struct TestS : public SerializableObject
{
	ZCVal(int,					v1);
	ZCVal(string,				v2);
};

struct TestS1 : public SerializableObject
{
	ZCVal(int,					id);
	ZCVal(TestS,				s);
};


int main() {
	TestS1 ts;

	ts.id = 123456;
	ts.s.v1 = 0;
	ts.s.v2 = "show";


	mongodb_iarchiver_type iar;
	iar.setIgnoreNull();
	iar & ts;

	printf("%s\n", iar.buffer().data());

	return 0;
}
