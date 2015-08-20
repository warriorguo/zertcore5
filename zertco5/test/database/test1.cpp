/*
 * test1.cpp
 *
 *  Created on: 2015Äê1ÔÂ15ÈÕ
 *      Author: Administrator
 */

#include <core/Runtime.h>
#include <concurrent/Concurrent.h>

#include <serialize/Serializer.h>
#include <serialize/Unserializer.h>

#include <db/Database.h>

#include <utils/time/CPUTime.h>

using namespace zertcore;
using namespace zertcore::db;

struct ExpiredValue : Unserializable<ExpiredValue>
{
	u32					value;
	time_type			last_update;

	template <class Archiver>
	bool unserialize(Archiver& achiver) {
		return	(achiver["value"] & value) &&
				(achiver["last_update"] & last_update);
	}
};

struct Solder : Unserializable<Solder>
{
	u32					type;
	u32					amount;

	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		return	(archiver["type"] & type) &&
				(archiver["amount"] & amount);
	}
};

int main() {
	mongodb::MongoDBAdapterConfig db_config;
	db_config.host = "192.168.1.53";
	mongodb::Database db;

	if (!db.init(db_config)) {
		::printf("Init failed\n");
	}
	db.selectDB("s1");

	mongodb::Database::query_type query;
	mongodb::Database::output_data_type output;

	query.equ("_id", 10000003);

	time::CPUTime::init();

	for (uint i = 0; i < 1000000; ++i) {
		cpu_time_type time1(Now);
		if (!db.queryOne("worldplayer", output, query)) {
			::printf("Query Failed\n");
			break;
		}

		cpu_time_type time2(Now);
		cpu_time_type offset = time2 - time1;

		if (offset.value() > 100000000) {
			::printf("Take:%ld\n", offset.value());
		}
	}

	/**
	if (!db.queryOne("worldplayer", output, query)) {
		::printf("Query Failed\n");
	}
	else {
		cpu_time_type time2(Now);
		::printf("Take Time:%ld time1:%ld time2:%ld\n", (time2 - time1).value(), time1.value(), time2.value());
		::printf("OUTPUT:\n%s\n\n\n", output.stream().data().toString().c_str());

		string name;
		output["name"] & name;
		u32 crystal;
		output["crystal"] & crystal;
		u32 gold;
		output["gold"] & gold;

		list<Solder> solders;
		ZC_ASSERT(output["total_solders"] & solders);

		::printf("Name:%s\nCrystal:%u\nGold:%u\n", name.c_str(), crystal, gold);

		::printf("Size:%u\n", solders.size());
		for (auto it = solders.begin(); it != solders.end(); ++it) {
			::printf("TYPE:%u AMOUNT:%u\n", it->type, it->amount);
		}

		ExpiredValue hero_action_point;
		ZC_ASSERT(output["hero_action_point"] & hero_action_point);

		::printf("Value:%u, LastUpdate:%f\n", hero_action_point.value, hero_action_point.last_update.value);
	}
	*/

	return 0;
}

