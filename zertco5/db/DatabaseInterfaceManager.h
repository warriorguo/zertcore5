/*
 * DatabaseManager.h
 *
 *  Created on: 2014Äê11ÔÂ24ÈÕ
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
	template <typename T>
	database_ptr fetchById(const T& id);

public:
};

}}


#endif /* DATABASEMANAGER_H_ */
