/*
 * DatabaseInstance.h
 *
 *  Created on: 2014Äê5ÔÂ1ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_DATABASEINSTANCE_H_
#define ZERTCORE_DATABASEINSTANCE_H_

#include <pch.h>
#include <db/Query.h>

namespace zertcore { namespace db {

/**
 * Database Interface
 */
template <class Adapter, class Cursor, class InputDataPackage,
	class OutputDataPackage, class AdapterConfig, class QueryDataType>
class DatabaseInterface
{
public:
	typedef Adapter							adapter_type;
	typedef Cursor							cursor_type;
	typedef AdapterConfig					adapter_config_type;
	typedef OutputDataPackage				output_data_type;
	typedef InputDataPackage				input_data_type;

public:
	typedef DatabaseInterface*				ptr;

public:
	typedef typename cursor_type::ptr		cursor_ptr;

public:
	typedef QueryDataType					query_type;

public:
	DatabaseInterface() {
		// TODO Auto-generated constructor stub
	}
	virtual ~DatabaseInterface() {
		// TODO Auto-generated destructor stub
	}

public:
	bool init(adapter_config_type& adapter_config);

public:
	bool selectDB(const string& db);
	string getSelectDB() const;

public:
	bool replace(const string& table_name, const input_data_type& data, const query_type& query = query_type());
	bool update(const string& table_name, const input_data_type& data, const query_type& query);

public:
	bool insert(const string& table_name, const input_data_type& data);
	bool remove(const string& table_name, const input_data_type& data, const query_type& query);

public:
	bool queryOne(const string& table_name, output_data_type& data, const query_type& query);
	bool query(const string& table_name, cursor_ptr& cursor, const query_type& query);
	bool getQueryData(cursor_ptr cursor, output_data_type& data);

private:
	string						select_db_;

private:
	adapter_type				adapter_;
};

}}

#include <db/details/DatabaseInterfaceImpl.hpp>

#endif /* DATABASEINSTANCE_H_ */
