/*
 * test1.cpp
 *
 *  Created on: 2014-4-26
 *      Author: Administrator
 */
#include <serialize/Serialize.h>

namespace t {

class T:
		public zertcore::serialization::Serializer<T>
{
public:
	virtual T& operator[] (const std::string& key) {
		printf("key:%s ", key.c_str());
		return *this;
	}
};

struct Tar
{
	int							a, b;
	std::string					str;
};

struct Tarc : zertcore::Serializable
{
	std::string					str;

	Tarc() : str("hello world") {}

	template <typename HostType>
	bool serialize(HostType& host) const {
		host("test1") & str;
		return true;
	}
};

}

namespace zertcore {
using namespace t;

template <typename HostType>
bool serialize(HostType& host, const Tar& value) {
	serialize(host, value.a);
	serialize(host, value.b);
	serialize(host, value.str);
	return true;
}

}

namespace zertcore {

template <>
bool serialize(T& host, const char* value) {
	printf("string : %s\n", value);
	return true;
}

template <>
bool serialize(T& host, const char& value) {
	printf("char : %c\n", value);
	return true;
}

template <>
bool serialize(T& host, const unsigned char& value) {
	printf("unsigned char : %c\n", value);
	return true;
}

template <>
bool serialize(T& host, const short& value) {
	printf("short : %d\n", value);
	return true;
}

template <>
bool serialize(T& host, const unsigned short& value) {
	printf("unsigned short : %d\n", value);
	return true;
}

template <>
bool serialize(T& host, const int& value) {
	printf("int : %d\n", value);
	return true;
}

template <>
bool serialize(T& host, const unsigned int& value) {
	printf("unsigned int : %u\n", value);
	return true;
}

template <>
bool serialize(T& host, const long& value) {
	printf("long : %ld\n", value);
	return true;
}

template <>
bool serialize(T& host, const unsigned long& value) {
	printf("unsigned long : %ld\n", value);
	return true;
}

template <>
bool serialize(T& host, const long long& value) {
	printf("long long : %ld\n", value);
	return true;
}

template <>
bool serialize(T& host, const unsigned long long& value) {
	printf("unsigned long long : %ld\n", value);
	return true;
}

template <>
bool serialize(T& host, const float& value) {
	printf("float : %f\n", value);
	return true;
}

template <>
bool serialize(T& host, const double& value) {
	printf("double : %f\n", value);
	return true;
}

template <>
bool serialize(T& host, const std::string& value) {
	printf("string : %s\n", value.c_str());
	return true;
}

}

int main() {
	using namespace t;

	Tarc tar;
	T s;
/**
	s("test1") & 1;
	s("test2") & 1.1;
	s("test3") & "show me the money";
*/
	s & tar;

	/**
	t::T test;
	_Node<t::T> s(test);

	s("test1") & 1;
	s("test2") & 1.1;
	s("test3") & "show me the money";

	std::list<std::string> ls;
	ls.push_back("operation cwal");
	ls.push_back("food for thought");

	s("test4") & ls;

	std::set<int> st;
	st.insert(123);
	st.insert(456);
	st.insert(789);

	s("test5") & st;

	std::map<int, float> mp;
	mp[1] = 1.1f;
	mp[2] = 1.2f;
	mp[3] = 1.3f;

	s("test6") & mp;

	t::Tar tar;
	tar.a = 1;
	tar.b = 2;
	tar.str = "winter is coming";
	s("test7") & tar;

	t::Tarc tarc;
	s("test8") & tarc;
	*/

	return 0;
}



