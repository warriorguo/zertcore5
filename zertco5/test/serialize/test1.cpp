/*
 * test1.cpp
 *
 *  Created on: 2014-4-29
 *      Author: Administrator
 */
#include <cstdio>
#include <string>

#include <serialize/Archive.h>
#include <serialize/Serialize.h>

namespace test {

class DerivedArchiver :
		public zertcore::serialization::IArchiver<DerivedArchiver>
{
public:
};

struct PlayerInfo :
		public zertcore::serialization::Serializable<PlayerInfo>
{
	int							id;
	std::string					name;

	template <class Archiver>
	bool serialize(Archiver& archiver) const {
		archiver("id") << id;
		archiver("name") << name;

		return true;
	}
};

struct VIPPlayerInfo :
		public PlayerInfo,
		public zertcore::serialization::Serializable<VIPPlayerInfo>
{
	int							level;

	template <class Archiver>
	bool serialize(Archiver& archiver) const {
		PlayerInfo::serialize(archiver);

		archiver("level") << level;
		return true;
	}
};

}

namespace zertcore { namespace serialization {
using namespace test;

template <>
inline bool operator << (DerivedArchiver& archiver, const char* value) {
	printf("<< %s\n", value);
	return true;
}

template <>
inline bool operator << (DerivedArchiver& archiver, const int& value) {
	printf("<< %d\n", value);
	return true;
}

template <>
inline bool operator << (DerivedArchiver& archiver, const std::string& value) {
	printf("<< %s\n", value.c_str());
	return true;
}

}}

int main() {
	test::DerivedArchiver derived;
	test::VIPPlayerInfo info;

	// test 1
	derived << 1;
	derived << "show me the money";

	// test 2
	std::map<int, std::string> test_map;
	test_map[1] = "operation cwal";
	test_map[2] = "food for thought";

	derived << test_map;

	// test 3
	info.id = 10001;
	info.level = 10;
	info.name = "Mr. Nobody";

	derived << info;

	return 0;
}
