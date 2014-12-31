/*
 * test2.cpp
 *
 *  Created on: 2014Äê4ÔÂ29ÈÕ
 *      Author: Administrator
 */
#include <cstdio>
#include <string>

#include <serialize/Archive.h>
#include <serialize/Unserialize.h>

namespace test {

class DerivedArchiver :
		public zertcore::serialization::OArchiver<DerivedArchiver>
{
public:
	DerivedArchiver() : count_(5) {}

public:
	virtual bool begin(DerivedArchiver& oar) {
		oar = *this;
		count_ = 5;
		return true;
	}
	virtual bool isValid() {
		return count_ >= 0;
	}
	virtual bool next() {
		count_--;
		return count_ >= 0;
	}
public:
	virtual zertcore::serialization::key_type key() {
		return "key" + boost::lexical_cast<std::string>(count_);
	}

private:
	int							count_;
};

struct PlayerInfo :
		public zertcore::serialization::Unserializable<PlayerInfo>
{
	int							id;
	std::string					name;

	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		archiver("id") >> id;
		archiver("name") >> name;

		return true;
	}
};

struct VIPPlayerInfo :
		public PlayerInfo,
		public zertcore::serialization::Unserializable<VIPPlayerInfo>
{
	int							level;

	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		PlayerInfo::unserialize(archiver);

		archiver("level") >> level;
		return true;
	}
};

}

namespace zertcore { namespace serialization {
using namespace test;

template <>
inline bool operator >> (DerivedArchiver& archiver, int& value) {
	value = 123456;
	return true;
}

template <>
inline bool operator >> (DerivedArchiver& archiver, std::string& value) {
	value = "show me the money";
	return true;
}

}}

int main() {
	test::DerivedArchiver derived;

	//test 1
	int i;
	derived >> i;
	std::string str;
	derived >> str;

	printf("%s -> %d\n", str.c_str(), i);

	//test 2
	std::map<std::string, int> m;
	derived >> m;

	for (std::map<std::string, int>::iterator it = m.begin(); it != m.end(); ++it) {
		printf("%s->%d\n", it->first.c_str(), it->second);
	}

	//test 3
	test::VIPPlayerInfo info;
	derived >> info;

	printf("id:%d\n name:%s\n level:%d\n", info.id, info.name.c_str(), info.level);

	return 0;
}
