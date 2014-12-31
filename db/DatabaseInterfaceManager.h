/*
 * DatabaseManager.h
 *
 *  Created on: 2014Äê11ÔÂ24ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_DATABASEMANAGER_H_
#define ZERTCORE_DATABASEMANAGER_H_

#include <pch.h>

#include <utils/SingleInstance.h>
#include <db/DatabaseInterface.h>

namespace zertcore { namespace db {
using namespace zertcore::utils;
}}

namespace zertcore { namespace db {

/**
 * DatabaseInterfaceManager
 */
template <class Adapter, class Cursor, class InputDataPackage,
	class OutputDataPackage, class AdapterConfig, class QueryDataType>
class DatabaseInterfaceManager :
		public SingleInstance<DatabaseInterfaceManager<Adapter, Cursor,
		InputDataPackage, OutputDataPackage, AdapterConfig, QueryDataType> >
{
public:
	typedef DatabaseInterface<Adapter, Cursor,
			InputDataPackage, OutputDataPackage, AdapterConfig, QueryDataType>
											database_type;
	typedef typename database_type::ptr		database_ptr;

public:
	template <typename T>
	database_ptr fetchById(const T& id);

public:
};

}}


#endif /* DATABASEMANAGER_H_ */
