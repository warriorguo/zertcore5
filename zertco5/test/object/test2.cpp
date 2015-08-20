/*
 * test2.cpp
 *
 *  Created on: 2015年6月17日
 *      Author: Administrator
 */

#include <zertcore.h>
#include <object/WorldObject.h>
#include <object/ActiveObjectManager.h>
#include <db/mongodb/details/MongoDBDataProvider.h>

namespace zertcore { namespace object {
using namespace zertcore::db;
class Player;
}}

namespace zertcore { namespace object {

template <>
struct ActiveObjectTraits<Player> : public ObjectTraits<Player>
{
	typedef uuid_t							id_type;

	static const char*						TABLE_NAME;
	static const char*						RPC_NAME;

	static const tick_type					DefaultExpiredTick; // default 10 second
};

const char*									ActiveObjectTraits<Player>::TABLE_NAME = "test_tb";
const char*									ActiveObjectTraits<Player>::RPC_NAME = "test";
const tick_type								ActiveObjectTraits<Player>::DefaultExpiredTick = 10000;

}}

namespace zertcore { namespace object {

class Player :
		public WorldObject<Player>
{
public:
	virtual bool init() {return true;}

public:
	string getName() const {
		return name;
	}
	void setName(const string& s) {
		name = s;
	}

private:
	ZCVal(string,				name);
	ZCVal(u32,					age);

	ZC_TO_STRING("age" << age << "name" << name);
};

/**
 * PlayerManager
 */
class PlayerManager :
		public WorldObjectManager<PlayerManager, Player>
{
public:
	enum {
		THREAD_INDEX						= 1,
	};
};

}}

using namespace zertcore;
using namespace zertcore::object;
using namespace zertcore::db;

int main() {
	mongodb::Database::adapter_config_type db_config;
	db_config.host = "10.0.0.165";
	db_config.db = "test";

	config.concurrent.thread_nums = 3;

	RT.globalInit([&]() {
		ZC_ASSERT( mongodb::DatabaseManager::Instance().add(db_config) );

		PlayerManager::go([] () {
			ConcurrentState::ptr state = ConcurrentState::create([] (RuntimeContext::ptr ctx) {
				if (ctx && ctx->error) {
					ZCLOG(NOTICE) << "Error:" << ctx->error.message << End;
					return ;
				}

				Player::ptr player = PlayerManager::Instance().get(1);
				if (!player) {
					ZCLOG(NOTICE) << "1 obj was null" << End;
					return ;
				}

				ZCLOG(NOTE) << "Name:" << player->getName() << End;

				player = PlayerManager::Instance().get(2);
				if (!player) {
					ZCLOG(NOTICE) << "2 obj was null" << End;
					return ;
				}

				ZCLOG(NOTE) << "Name:" << player->getName() << End;

				ConcurrentState::ptr state = ConcurrentState::create([] (RuntimeContext::ptr ctx) {
					Player::ptr obj = PlayerManager::Instance().get(1);
					ZCLOG(NOTE) << "In Name:" << obj->getName() << End;
				});

				PlayerManager::Instance().prepare(1, 0, state);
			});

			PlayerManager::Instance().prepare(1, 0, state);
			PlayerManager::Instance().prepare(2, 0, state);
		});
	});
	RT.run();

	return 0;
}

