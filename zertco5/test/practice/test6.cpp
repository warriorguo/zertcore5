/*
 * test6.cpp
 *
 *  Created on: 2015年4月27日
 *      Author: Administrator
 */

#include <cstdio>
#include <boost/bimap/bimap.hpp>

template <class Final>
class Base
{
public:
	typedef Final							self;

public:
};

class F : public Base<F>
{
public:
	void work() {
		bind(&self::work, this);
	}
};

int main() {
	return 0;
}


