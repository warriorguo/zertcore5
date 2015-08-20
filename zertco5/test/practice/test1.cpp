/*
 * test1.cpp
 *
 *  Created on: 2014Äê4ÔÂ29ÈÕ
 *      Author: Administrator
 */
#include <cstdio>

namespace t1{

template <typename T>
class Test
{
	template <typename R, typename V>
	friend void operator << (R& archiver, const V& v);
protected:
	int							a;
};


template <typename T, typename V>
void operator << (T& archiver, const V& v);

template <typename T>
void operator << (T& archiver, const int& v);

}


namespace t2{

class Derived : public t1::Test<Derived>
{
private:
	int							b;
};
}

namespace t1{
using namespace t2;

template <>
void operator << (Derived& t, const int& v) {
	t.a = v;
	t.b = v;
}
}

int main() {
	t2::Derived t;
	t << 1;

	return 0;
}

