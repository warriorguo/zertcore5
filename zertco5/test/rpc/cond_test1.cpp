/*
 * cond_test1.cpp
 *
 *  Created on: 2015年6月29日
 *      Author: Administrator
 */

#include <zertcore.h>

using namespace zertcore;
using namespace zertcore::concurrent;

bool test(const rpc::condition_expr_type& expr, rpc::oarchiver_type& v) {
	return expr(v);
}

int main() {
	rpc::iarchiver_type iar;
	iar["test"] & 8;

	rpc::oarchiver_type oar(iar);

	if (test(rpc::condition("test", cond::BETWEEN, 5, 8), oar)) {
		::printf("yes\n");
	}
	else {
		::printf("no\n");
	}

	return 0;
}


