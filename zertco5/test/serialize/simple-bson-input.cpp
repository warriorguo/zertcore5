/*
 * simple-bson.cpp
 *
 *  Created on: 2014Äê4ÔÂ29ÈÕ
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

class InputBSONArchiver:
		public IArchiver<InputBSONArchiver>
{
public:
	struct Cell
	{
		enum {
			OBJ, LIST
		};
		Cell() : type(OBJ) {};

		int						type;
		BSONObjBuilder			obj;
		BSONArrayBuilder		array;
	};
	typedef map<key_type, InputBSONArchiver>
											archiver_map_type;

public:
	InputBSONArchiver() {}
	InputBSONArchiver(const InputBSONArchiver&) {}
	virtual ~InputBSONArchiver() {}

public:
	BSONObj data() {
		BSONObjBuilder ob;
		if (cell_.type == Cell::OBJ) {
			ob.appendElements(cell_.obj.obj());
		}
		else {
			ob.appendElements(cell_.array.obj());
		}

		for (archiver_map_type::iterator it = archiver_map_.begin();
				it != archiver_map_.end(); ++it) {
			ob.append(it->first, it->second.data());
		}

		return ob.obj();
	}

public:
	void setList() {
		cell_.type = Cell::LIST;
	}
	void setObject() {
		cell_.type = Cell::OBJ;
	}

	Cell& getCell() {
		return cell_;
	}

public:
	virtual InputBSONArchiver& list() {
		if (key_.empty())
			return *this;

		InputBSONArchiver& ar(archiver_map_[key_]);
		ar.setList();
		return ar;
	}
	virtual InputBSONArchiver& object() {
		if (key_.empty())
			return *this;

		InputBSONArchiver& ar = archiver_map_[key_];
		ar.setObject();
		return ar;
	}

private:
	Cell						cell_;
	archiver_map_type			archiver_map_;
};


////////////////////////////////////

/**
 * Test Structures
 */
struct PlayerInfo :
		public zertcore::serialization::Serializable<PlayerInfo>
{
	int							id;
	std::string					name;
	map<std::string, int>		tags;

	template <class Archiver>
	bool serialize(Archiver& archiver) const {
		archiver("id") & id;
		archiver("name") & name;
		archiver("tags") & tags;

		return true;
	}
};

struct VIPPlayerInfo :
		public PlayerInfo,
		public zertcore::serialization::Serializable<VIPPlayerInfo>
{
	int							level;
	std::list<int>				list;

	template <class Archiver>
	bool serialize(Archiver& archiver) const {
		PlayerInfo::serialize(archiver);

		archiver("level") & level;
		archiver("list") & list;

		return true;
	}
};

}

namespace zertcore { namespace serialization {
using namespace test;

template <>
inline bool operator << (InputBSONArchiver& archiver, const int& value) {
	if (archiver.getCell().type == InputBSONArchiver::Cell::LIST) {
		archiver.getCell().array.append(value);
	}
	else {
		archiver.getCell().obj.append(archiver.key_, value);
	}
	return true;
}

template <>
inline bool operator << (InputBSONArchiver& archiver, const std::string& value) {
	if (archiver.getCell().type == InputBSONArchiver::Cell::LIST) {
		archiver.getCell().array.append(value);
	}
	else {
		archiver.getCell().obj.append(archiver.key_, value);
	}
	return true;
}

}}

int main() {
	test::InputBSONArchiver derived;
	test::VIPPlayerInfo info;

	info.id = 10001;
	info.level = 10;
	info.name = "Mr. Nobody";

	info.tags["happy"] = 2;
	info.tags["lovable"] = 10;

	info.list.push_back(1);
	info.list.push_back(2);
	info.list.push_back(3);

	derived("player1") & info;
	derived("player2") & info;

	printf("BSON:%s\n", derived.data().jsonString().c_str());

	return 0;
}
