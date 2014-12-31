/*
 * DatabaseInterfaceImpl.hpp
 *
 *  Created on: 2014Äê11ÔÂ10ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_DATABASEINTERFACEIMPL_HPP_
#define ZERTCORE_DATABASEINTERFACEIMPL_HPP_

namespace zertcore{ namespace db{

template <class Adapter, class Cursor, class InputDataPackage,
	class OutputDataPackage, class AdapterConfig, class QueryDataType>
inline bool DatabaseInterface<Adapter, Cursor, InputDataPackage, OutputDataPackage, AdapterConfig, QueryDataType>::
init(adapter_config_type& adapter_config) {
	return adapter_.init(adapter_config);
}

template <class Adapter, class Cursor, class InputDataPackage,
	class OutputDataPackage, class AdapterConfig, class QueryDataType>
inline bool DatabaseInterface<Adapter, Cursor, InputDataPackage, OutputDataPackage, AdapterConfig, QueryDataType>::
selectDB(const string& db) {
	select_db_ = db;
	return adapter_.selectDB(db);
}

template <class Adapter, class Cursor, class InputDataPackage,
	class OutputDataPackage, class AdapterConfig, class QueryDataType>
inline string DatabaseInterface<Adapter, Cursor, InputDataPackage, OutputDataPackage, AdapterConfig, QueryDataType>::
getSelectDB() const {
	return select_db_;
}

template <class Adapter, class Cursor, class InputDataPackage,
	class OutputDataPackage, class AdapterConfig, class QueryDataType>
inline bool DatabaseInterface<Adapter, Cursor, InputDataPackage, OutputDataPackage, AdapterConfig, QueryDataType>::
replace(const string& table_name, const input_data_type& data, const query_type& query) {
	return adapter_.replace(table_name, data, query);
}

template <class Adapter, class Cursor, class InputDataPackage,
	class OutputDataPackage, class AdapterConfig, class QueryDataType>
inline bool DatabaseInterface<Adapter, Cursor, InputDataPackage, OutputDataPackage, AdapterConfig, QueryDataType>::
update(const string& table_name, const input_data_type& data, const query_type& query) {
	return adapter_.update(table_name, data, query);
}

template <class Adapter, class Cursor, class InputDataPackage,
	class OutputDataPackage, class AdapterConfig, class QueryDataType>
inline bool DatabaseInterface<Adapter, Cursor, InputDataPackage, OutputDataPackage, AdapterConfig, QueryDataType>::
insert(const string& table_name, const input_data_type& data) {
	return adapter_.save(table_name, data);
}

template <class Adapter, class Cursor, class InputDataPackage,
	class OutputDataPackage, class AdapterConfig, class QueryDataType>
inline bool DatabaseInterface<Adapter, Cursor, InputDataPackage, OutputDataPackage, AdapterConfig, QueryDataType>::
remove(const string& table_name, const input_data_type& data, const query_type& query) {
	return adapter_.remove(table_name, data, query);
}

template <class Adapter, class Cursor, class InputDataPackage,
	class OutputDataPackage, class AdapterConfig, class QueryDataType>
inline bool DatabaseInterface<Adapter, Cursor, InputDataPackage, OutputDataPackage, AdapterConfig, QueryDataType>::
queryOne(const string& table_name, output_data_type& data, const query_type& query) {
	return adapter_.queryOne(table_name, data, query);
}

template <class Adapter, class Cursor, class InputDataPackage,
	class OutputDataPackage, class AdapterConfig, class QueryDataType>
inline bool DatabaseInterface<Adapter, Cursor, InputDataPackage, OutputDataPackage, AdapterConfig, QueryDataType>::
query(const string& table_name, cursor_ptr& cursor, const query_type& query) {
	return adapter_.query(table_name, cursor, query);
}

template <class Adapter, class Cursor, class InputDataPackage,
	class OutputDataPackage, class AdapterConfig, class QueryDataType>
inline bool DatabaseInterface<Adapter, Cursor, InputDataPackage, OutputDataPackage, AdapterConfig, QueryDataType>::
getQueryData(cursor_ptr cursor, output_data_type& data) {
	return adapter_.getQueryData(cursor, data);
}

}}


#endif /* DATABASEINTERFACEIMPL_HPP_ */
