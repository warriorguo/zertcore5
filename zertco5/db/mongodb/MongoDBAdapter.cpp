/*
 * MongoDBAdapter.cpp
 *
 *  Created on: 2014��5��3��
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
		ZCLOG(FATAL) << "Failed to Connect Database:" << config << " ErrorMsg:" << errmsg << End;
		return false;
	}

	return true;
}

bool MongoDBAdapter::
replace(const string& table_name, const input_data_type& data, const query_type& query) {
	BSONObjBuilder ob;
	ob.appendElements(data.stream().data()).appendElementsUnique(query.getData().stream().data());

	conn_.update(ns_ + table_name, query.getData().stream().data(), ob.obj(), true, false);
	return true;
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
	time_type before_now(Now);
	BSONObj rs = conn_.findOne(ns_ + table_name, query.getData().stream().data());
	time_type after_now(Now);

	time_type::type offset = after_now.value - before_now.value;
	if (offset >= 0.01) {
		ZCLOG(NOTICE) << "MongoDB find one was too slow, took " << offset << " seconds to findone" << End;
	}

	if (rs.isEmpty())
		return false;

	data.stream().data(rs);
	return true;
}

bool MongoDBAdapter::
query(const string& table_name, cursor_ptr& cursor, const query_type& query) {
	::mongo::Query q(query.getData().stream().data());

	const query::order_map_type& orders = query.getOrders();
	if (!orders.empty()) {
		for (auto it = orders.begin(); it != orders.end(); ++it) {
			switch (it->second) {
			case ORDER_ASC:
				q.sort(it->first, 1);
				break;
			case ORDER_DESC:
				q.sort(it->first, -1);
				break;
			}
		}
	}

	auto_ptr<DBClientCursor> c = conn_.query(ns_ + table_name, q,
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
