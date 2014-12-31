/*
 * test2.cpp
 *
 *  Created on: 2014-4-29
 *      Author: Administrator
 */
#include <boost/type_traits.hpp>
#include <boost/utility.hpp>

using namespace boost;

struct Serializable{};

struct Derived : Serializable {};

class Serializer
{
public:
	template <typename ValueType, typename = typename enable_if<is_base_of<Serializable, ValueType>, ValueType>::type>
	void operator& (const ValueType& value) {
		;
	}
};

int main() {
	Serializer serializer;
	Derived tar;

	serializer & tar;

	return 0;
}



