/*
 * bsonstream_test1.cpp
 *
 *  Created on: 2015��1��19��
 *      Author: Administrator
 */

#include <serialize/Serializer.h>
#include <serialize/Unserializer.h>
#include <db/mongodb/serialize/BSONStream.h>
#include <utils/time/TimeType.h>

using namespace zertcore;
using namespace zertcore::serialization;
using namespace zertcore::db::mongodb::serialization;

struct Data : Serializable<Data>, Unserializable<Data>
{
	string						key;
	double						value;

	template <class Archiver>
	void serialize(Archiver& archiver) const {
		archiver["key"] & key;
		archiver["value"] & value;
	}

	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		archiver["key"] & key;
		archiver["value"] & value;

		return true;
	}
};

struct Test : Serializable<Test>, Unserializable<Test>
{
	u32							id;
	string						name;
	u8							age;

	list<Data>					data;

	template <class Archiver>
	void serialize(Archiver& archiver) const {
		archiver["id"] & id;
		archiver["name"] & name;
		archiver["age"] & age;
		archiver["data"] & data;
	}
	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		archiver["id"] & id;
		archiver["name"] & name;
		archiver["age"] & age;
		archiver["data"] & data;

		return true;
	}
};

void work() {
	Serializer<BSONIStream> bs;
	map<u32, string> zen;

	zen[10000] = "hahahaha";
	zen[20000] = "hehehehe";
	zen[30000] = "operation cwal";

	bs["show"] & 1;
	bs["me"] & 1.2;
	bs["the"] & "money";
	bs["money"] & zen;

	Test pp;
	pp.id = 6507;
	pp.name = "BBBBBBBBS";
	pp.age = 23;

	Data d;
	d.key = "meimei";
	d.value = 123;

	pp.data.push_back(d);

	d.key = "dada";
	d.value = 88888888;

	pp.data.push_back(d);

	bs["person"] & pp;

	Unserializer<BSONOStream> ubs;
//	ZC_ASSERT(ubs.parse(bs.getString()));

	map<u32, string> to_zen;

	u32 a;
	double b;
	string c;

	ubs["show"] & a;
	ubs["me"] & b;
	ubs["the"] & c;
	ubs["money"] & zen;

	ubs["person"] & pp;
}

int main() {

	time_type t1(Now);
	for (uint i = 0; i < 100000; ++i) work();
	time_type t2(Now);

	printf("take : %f\n", (t2 - t1).value);

	return 0;
}

