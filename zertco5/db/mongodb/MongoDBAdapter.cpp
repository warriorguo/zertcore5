/*
 * MongoDBAdapter.cpp
 *
 *  Created on: 2014Äê5ÔÂ3ÈÕ
 *      Author: Administrator
 */

#include <pch.h>

#include <log/Log.h>
#include "MongoDBAdapter.h"

namespace zertcore { namespace db { namespace mongodb{

bool MongoDBAdapter::
init(const config_type& config) {
	string host = config.host + ":" + lexical_cast<string>(config.port);
	string errmsg;

	if (!conn_.connect(host, errmsg)) {
		ZCLOG(FINAL) << "Failed to Connect Database:" << config << " ErrorMsg:" << errmsg << End;
		return false;
	}

	return true;
}

bool MongoDBAdapter::
replace(const string& table_name, const input_data_type& data, const query_type& query) {
	conn_.update(ns_ + table_name, query.getData().stream().data(), data.stream().data(), true, false);
	return false;
}

bool MongoDBAdapter::
update(const string& table_name, const input_data_type& data, const query_type& query) {
	conn_.update(ns_ + table_name, query.getData().stream().data(), data.stream().data());
	return true;
}

bool MongoDBAdapter::
insert(const string& table_name, const input_data_type& data) {
	conn_.insert(ns_ + table_name, data.stream().data());
	return true;
}

bool MongoDBAdapter::
remove(const string& table_name, const input_data_type& data, const query_type& query) {
	conn_.remove(ns_ + table_name, query.getData().stream().data(), query.getLimit() == 1);
	return true;
}

bool MongoDBAdapter::
queryOne(const string& table_name, output_data_type& data, const query_type& query) {
	BSONObj rs = conn_.findOne(ns_ + table_name, query.getData().stream().data());
	if (rs.isEmpty())
		return false;

	data.stream().data(rs);
	return true;
}

bool MongoDBAdapter::
query(const string& table_name, cursor_ptr& cursor, const query_type& query) {
	auto_ptr<DBClientCursor> c = conn_.query(ns_ + table_name, query.getData().stream().data(),
			query.getLimit(), query.getFrom());
	if (!c->more())
		return false;

	cursor = MongoDBCursor::create();
	cursor->setCursor(c);

	return true;
}

bool MongoDBAdapter::
getQueryData(cursor_ptr cursor, output_data_type& data) {
	if (!cursor)
		return false;

	return cursor->moveNext(data);
}

}}}
