/*
 * test7.cpp
 *
 *  Created on: 2015年6月1日
 *      Author: Administrator
 */

#include <cstdio>
#include <boost/type_traits.hpp>
#include <boost/utility.hpp>
#include <boost/bind.hpp>

using namespace boost;

class C
{
public:
	template <typename Callable>
	void push(Callable caller) {
		_push(is_member_function_pointer<Callable>(), caller);
	}

public:
	template <typename Callable>
	void _push(const true_type& _, Callable caller) {
		typedef decltype((this->*caller)()) r_type;
		bind(caller, this)();
	}

	template <typename Callable>
	void _push(const false_type& _, Callable caller) {
		bind(caller)();
	}

public:
	void test() {printf("test\n");}

private:
};

int main() {
	C c;
	c.push(&C::test);

	return 0;
}


