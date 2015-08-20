/*
 * MongoDBAdapter.h
 *
 *  Created on: 2014Äê5ÔÂ3ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_MONGODBADAPTER_H_
#define ZERTCORE_MONGODBADAPTER_H_

#include <pch.h>

#include <serialize/Serializer.h>
#include <serialize/Unserializer.h>

#include "serialize/BSONStream.h"

#include "../Query.h"
#include "config.h"
#include "MongoDBCursor.h"
#include "MongoDBQuery.h"

namespace zertcore { namespace db { namespace mongodb{
using namespace zertcore::db::mongodb::serialization;
}}}

namespace zertcore { namespace db { namespace mongodb{
/**
 * Impl for mongodb
 */
class MongoDBAdapter
{
public:
	typedef Serializer<BSONIStream>			input_data_type;
	typedef Unserializer<BSONOStream>		output_data_type;
	typedef MongoDBCursor::ptr				cursor_ptr;
	typedef MongoDBQuery					query_type;
	typedef MongoDBAdapterConfig			config_type;

public:
	MongoDBAdapter() {
		// TODO Auto-generated constructor stub
	}

	virtual ~MongoDBAdapter() {
		// TODO Auto-generated destructor stub
	}

public:
	bool init(const config_type& config);
	bool selectDB(const string& db) {
		ns_ = db + ".";
		return true;
	}

public:
	bool replace(const string& table_name, const input_data_type& data, const query_type& query);
	bool update(const string& table_name, const input_data_type& data, const query_type& query);

public:
	bool insert(const string& table_name, const input_data_type& data);
	bool remove(const string& table_name, const input_data_type& data, const query_type& query);

public:
	bool queryOne(const string& table_name, output_data_type& data, const query_type& query);
	bool query(const string& table_name, cursor_ptr& cursor, const query_type& query);
	bool getQueryData(cursor_ptr cursor, output_data_type& data);

public:
	DBClientConnection& getConnection() {return conn_;}
	const DBClientConnection& getConnection() const {return conn_;}

private:
	string						ns_;

private:
	DBClientConnection			conn_;
};

}}}


#endif /* MONGODBADAPTER_H_ */
