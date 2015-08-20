/*
 * simple-bson-output.cpp
 *
 *  Created on: 2014Äê4ÔÂ30ÈÕ
 *      Author: Administrator
 */
#include <cstdio>
#include <string>

#include <serialize/Archive.h>
#include <serialize/Serialize.h>
#include <serialize/Unserialize.h>

#include <mongo/client/dbclient.h>

namespace test {
using namespace mongo;
using namespace zertcore::serialization;

class OutputBSONArchiver:
		public OArchiver<OutputBSONArchiver>
{
public:
	typedef map<key_type, OutputBSONArchiver>
											archiver_map_type;

public:
	OutputBSONArchiver() : iterator_(NULL, NULL) {}
	virtual ~OutputBSONArchiver() {}

public:
	OutputBSONArchiver& operator() (const key_type& key) {
		key_ = key;
		OutputBSONArchiver& ar = archiver_map_[key];

		ar.element_ = obj_[key];

		if (ar.element_.type() == 3) {
			ar.obj_ = ar.element_.Obj();
		}

		return ar;
	}

public:
	void setData(const BSONObj& obj) {
		obj_ = obj;
	}
	BSONObj& getData() {
		return obj_;
	}
	BSONElement& getElement() {
		if (!key_.empty()) {
			element_ = obj_.getField(key_);
		}

		return element_;
	}

public:
	virtual bool begin(OutputBSONArchiver& oar) {
		if (obj_.isEmpty()) return false;

		oar.iterator_ = obj_.begin();

		oar.element_ = oar.iterator_.next();
		if (oar.element_.type() == 3) {
			oar.obj_ = oar.element_.Obj();
		}

		return true;
	}
	virtual bool isValid() {
		return iterator_.moreWithEOO();
	}
	virtual bool next() {
		element_ = iterator_.next();

		if (element_.type() == 3) {
			obj_ = element_.Obj();
		}

		return true;
	}

public:
	virtual key_type key() {
		return element_.fieldName();
	}

private:
	archiver_map_type			archiver_map_;

private:
	BSONObjIterator				iterator_;
	BSONElement					element_;
	BSONObj						obj_;
};

////////////////////////////////////

/**
 * Test Structures
 */
struct PlayerInfo :
		public zertcore::serialization::Unserializable<PlayerInfo>
{
	int							id;
	std::string					name;
	map<std::string, int>		tags;

	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		archiver("id") & id;
		archiver("name") & name;
		archiver("tags") & tags;

		return true;
	}
};

struct VIPPlayerInfo :
		public PlayerInfo,
		public zertcore::serialization::Unserializable<VIPPlayerInfo>
{
	int							level;
	std::list<int>				list;

	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		PlayerInfo::unserialize(archiver);

		archiver("level") & level;
		archiver("list") & list;

		return true;
	}
};

}

namespace zertcore { namespace serialization {
using namespace test;

template <>
inline bool operator >> (OutputBSONArchiver& archiver, int& value) {
	value = 0;
	BSONElement& elemt = archiver.getElement();

	if (!elemt.ok()) {
		return false;
	}
	if (elemt.type() != NumberInt) {
		try {
			switch (elemt.type()) {
			case NumberDouble:
				value = (int)elemt.Double();
				return true;
			case String:
				value = lexical_cast<int>(elemt.String());
				return true;
			case NumberLong:
				value = int(elemt.Long());
				return true;
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}
		return false;
	}
	value = elemt.Int();

	return true;
}

template <>
inline bool operator >> (OutputBSONArchiver& archiver, std::string& value) {
	value.clear();
	BSONElement& elemt = archiver.getElement();

	if (!elemt.ok()) {
		return false;
	}
	if (elemt.type() != String) {
		try {
			switch (elemt.type()) {
			case NumberDouble:
				value = lexical_cast<string>(elemt.Double());
				return true;
			case NumberInt:
				value = lexical_cast<string>(elemt.Int());
				return true;
			case NumberLong:
				value = lexical_cast<string>(elemt.Long());
				return true;
			case jstOID:
				value = elemt.__oid ().str();
				return true;
			default:
				break;
			}
		}
		catch(bad_lexical_cast&) {
			return false;;
		}

		return false;
	}
	value = elemt.String();

	return true;
}

}}

int main() {
	test::OutputBSONArchiver derived;
	test::VIPPlayerInfo player1, player2;

	mongo::BSONObj data = mongo::fromjson("{\"player1\":{\"id\":10001,\"name\":\"Mr.Nobody\",\"level\":10,\"list\":{\"0\":1,\"1\":2,\"2\":3,\"3\":4},\"tags\":{\"happy\":2,\"happythere\":11,\"lovable\":10}},\"player2\":{\"id\":10002,\"name\":\"Mr.Nobody\",\"level\":10,\"list\":{\"0\":1,\"1\":2,\"2\":3},\"tags\":{\"happy\":2,\"lovable\":10}}}");
	derived.setData(data);

	derived("player1") & player1;
	derived("player2") & player2;

	printf("player1:%d %s %d list size:%ld tag size:%ld\n", player1.id, player1.name.c_str(), player1.level, player1.list.size(), player1.tags.size());

	for (std::map<std::string, int>::iterator it = player1.tags.begin(); it != player1.tags.end(); ++it) {
		printf("%s-%d\n", it->first.c_str(), it->second);
	}

	printf("player2:%d\n", player2.id);

	return 0;
}

