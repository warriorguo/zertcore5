/*
 * test4.cpp
 *
 *  Created on: 2015年2月4日
 *      Author: Administrator
 */

#include <pch.h>

namespace zertcore {

struct Callable
{
	int v;
	void operator() () {printf("%d\n", v);}
};

void call(const function<void ()>& handler) {
	handler();
}

}

using namespace zertcore;

int main() {
	std::vector<function<void ()> > list;
	Callable ca;
	ca.v = 0;
	list.push_back(ca);
	ca.v = 1;
	list.push_back(ca);
	ca.v = 2;
	list.push_back(ca);

	std::for_each(list.begin(), list.end(), &call);

	return 0;
}

