/*
 * test1.cpp
 *
 *  Created on: 2015年6月6日
 *      Author: Administrator
 */

#include <zertcore.h>

#include <object/ActiveObject.h>
#include <object/ActiveObjectTraits.h>

#include <object/ActiveObjectManager.h>
#include <db/mongodb/details/MongoDBDataProvider.h>

namespace zertcore { namespace object {
using namespace zertcore::db;
}}

namespace zertcore { namespace object {

class TestObject;

template <>
struct ActiveObjectTraits<TestObject> : public ObjectTraits<TestObject>
{
	typedef uuid_t							id_type;

	static const char*						TABLE_NAME;
	static const char*						RPC_NAME;
	static const char*						SYNC_NAME;

	static const tick_type					DefaultExpiredTick; // default 10 second
};

const char*									ActiveObjectTraits<TestObject>::TABLE_NAME = "test_tb";
const char*									ActiveObjectTraits<TestObject>::RPC_NAME = "test";
const tick_type								ActiveObjectTraits<TestObject>::DefaultExpiredTick = 100000;
const char*									ActiveObjectTraits<TestObject>::SYNC_NAME = NULL;

}}

namespace zertcore { namespace object {

class TestObject :
		public ActiveObject<TestObject>
{
public:
	virtual bool init() {return true;}

public:
	string getName() const {return name_;}
	void setName(const string& name) {name_ = name;}

public:
	template <class Archiver>
	void serialize(Archiver& archiver) const {
		archiver["age"] & age_;
		archiver["name"] & name_;
	}

	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		return
			(archiver["age"] & age_) &&
			(archiver["name"] & name_);
	}

private:
	u32							age_;
	string						name_;

	ZC_TO_STRING("age" << age_ << "name" << name_)
};

class TestObjectManager : public ActiveObjectManager<TestObjectManager, TestObject>
{
public:
	enum {
		THREAD_INDEX						= 2,
	};
public:
	virtual void init() {
		ActiveObjectManager<TestObjectManager, TestObject>::init();

		dp_manager::Instance().reg(1, new io::MongoDBDataProvider<TestObject>);
	}

public:
	virtual tid_type getThreadIndex() const {return Thread::lazyTid(1); }
};

}}

using namespace zertcore;
using namespace zertcore::object;
using namespace zertcore::db;

int main() {
	mongodb::Database::adapter_config_type db_config;
	db_config.host = "192.168.1.11";
//	db_config.host = "10.0.0.174";
	db_config.db = "test";

	config.concurrent.thread_nums = 3;

	RT.globalInit([&]() {
		ZC_ASSERT( mongodb::DatabaseManager::Instance().add(db_config) );
/**
		time_type begin_time(Now);
		for (uuid_t i = 1; i <= 10000; ++i) {
			mongodb::DatabaseManager::Instance().fetchById(i)->adapter().getConnection().findOne("test.test_tb", BSON("_id" << (long long)i));
		}
		time_type end_time(Now);

		::printf("take time:%f\n", end_time.value - begin_time.value);

		return ;
*/
		for (uuid_t i = 1; i <= 2; ++i)
		TestObjectManager::go([i] () {
			/**
			ConcurrentState::ptr state = ConcurrentState::create([] (RuntimeContext::ptr ctx) {
				if (ctx && ctx->error) {
					ZCLOG(NOTICE) << "Error:" << ctx->error.message << End;
					return ;
				}

				TestObject::ptr obj = TestObjectManager::Instance().get(1);
				if (!obj) {
					ZCLOG(NOTICE) << "1 obj was null" << End;
					return ;
				}

				ZCLOG(NOTE) << "Name:" << obj->getName() << End;

				obj = TestObjectManager::Instance().get(2);
				if (!obj) {
					ZCLOG(NOTICE) << "2 obj was null" << End;
					return ;
				}

				ZCLOG(NOTE) << "Name:" << obj->getName() << End;
			});

			TestObjectManager::Instance().prepare(1, 0, state);
			TestObjectManager::Instance().prepare(2, 0, state);
			*/
			//	TestObjectManager::Instance().prepare(i, 1);
				uuid_t id = i;
				TestObjectManager::Instance().get(id, 1);
				/**TestObject::ptr obj = TestObjectManager::Instance().get(id, 1);*/
				if (id == 10000) {
					RT.stop();

					/**
					obj = TestObjectManager::Instance().create(i, 1);

					std::stringstream ss;

					ss << "Name-" << i;

					obj->setName(ss.str());
					obj->save();
					*/
					//ZCLOG(NOTE) << obj->getName() << End;
				}
		});

/**
		TestObject::ptr obj = TestObjectManager::Instance().create(1, 0);
		if (!obj) {
			ZCLOG(ERROR) << "Create Failed" << End;
		}

		TestObjectManager::Instance().save(1);
*/
	});
	RT.run();

	return 0;
}
