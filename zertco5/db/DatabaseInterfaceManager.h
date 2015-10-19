/*
 * DatabaseManager.h
 *
 *  Created on: 2014��11��24��
 *      Author: Administrator
 */

#ifndef ZERTCORE_DATABASEMANAGER_H_
#define ZERTCORE_DATABASEMANAGER_H_

#include <pch.h>

#include <utils/Singleton.h>
#include <db/DatabaseInterface.h>

namespace zertcore { namespace db {
using namespace zertcore::utils;
}}

namespace zertcore { namespace db {

/**
 * DatabaseInterfaceManager
 */
template <class Adapter>
class DatabaseInterfaceManager :
		public Singleton<DatabaseInterfaceManager<Adapter> >
{
public:
	typedef DatabaseInterface<Adapter>
											database_type;
	typedef typename database_type::ptr		database_ptr;

public:
	typedef vector<database_ptr>			database_slot_type;

public:
	template <typename T>
	database_ptr fetchById(const T& id) {
		if (database_slot_.empty())
			return nullptr;

		hash<T> hasher;
		return database_slot_[hasher(id) % database_slot_.size()];
	}

public:
	bool add(typename database_type::adapter_config_type& config) {
		database_ptr db = new database_type;
		if (!db->init(config))
			return false;

		database_slot_.push_back(db);
		return true;
	}

private:
	database_slot_type			database_slot_;
};

}}


#endif /* DATABASEMANAGER_H_ */
