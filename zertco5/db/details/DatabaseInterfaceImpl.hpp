/*
 * DatabaseInterfaceImpl.hpp
 *
 *  Created on: 2014Äê11ÔÂ10ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_DATABASEINTERFACEIMPL_HPP_
#define ZERTCORE_DATABASEINTERFACEIMPL_HPP_

namespace zertcore{ namespace db{

template <class Adapter>
inline bool DatabaseInterface<Adapter>::
init(adapter_config_type& adapter_config) {
	return adapter_.init(adapter_config);
}

template <class Adapter>
inline bool DatabaseInterface<Adapter>::
selectDB(const string& db) {
	select_db_ = db;
	return adapter_.selectDB(db);
}

template <class Adapter>
inline string DatabaseInterface<Adapter>::
getSelectDB() const {
	return select_db_;
}

template <class Adapter>
inline bool DatabaseInterface<Adapter>::
replace(const string& table_name, const input_data_type& data, const query_type& query) {
	return adapter_.replace(table_name, data, query);
}

template <class Adapter>
inline bool DatabaseInterface<Adapter>::
update(const string& table_name, const input_data_type& data, const query_type& query) {
	return adapter_.update(table_name, data, query);
}

template <class Adapter>
inline bool DatabaseInterface<Adapter>::
insert(const string& table_name, const input_data_type& data) {
	return adapter_.save(table_name, data);
}

template <class Adapter>
inline bool DatabaseInterface<Adapter>::
remove(const string& table_name, const input_data_type& data, const query_type& query) {
	return adapter_.remove(table_name, data, query);
}

template <class Adapter>
inline bool DatabaseInterface<Adapter>::
queryOne(const string& table_name, output_data_type& data, const query_type& query) {
	return adapter_.queryOne(table_name, data, query);
}

template <class Adapter>
inline bool DatabaseInterface<Adapter>::
query(const string& table_name, cursor_ptr& cursor, const query_type& query) {
	return adapter_.query(table_name, cursor, query);
}

template <class Adapter>
inline bool DatabaseInterface<Adapter>::
getQueryData(cursor_ptr cursor, output_data_type& data) {
	return adapter_.getQueryData(cursor, data);
}

}}


#endif /* DATABASEINTERFACEIMPL_HPP_ */
