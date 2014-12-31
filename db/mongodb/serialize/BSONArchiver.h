/*
 * BSONFormat.h
 *
 *  Created on: 2014Äê9ÔÂ16ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_BSONARCHIVER_H_
#define ZERTCORE_BSONARCHIVER_H_

#include <pch.h>

#include <serialize/Archive.h>
#include <serialize/Serialize.h>
#include <serialize/Unserialize.h>

#include <db/mongodb/MongoDBConfig.h>

namespace zertcore { namespace db { namespace mongo { namespace serialization {
using namespace zertcore::serialization;
}}}}


namespace zertcore { namespace db { namespace mongo { namespace serialization {

class BSONOArchiver:
		public OArchiver<BSONOArchiver>
{
public:
	typedef map<key_type, BSONOArchiver>	archiver_map_type;

public:
	BSONOArchiver() : iterator_(NULL, NULL) {}
	virtual ~BSONOArchiver() {}

public:
	BSONOArchiver& operator() (const key_type& key) {
		key_ = key;
		BSONOArchiver& ar = archiver_map_[key];

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
	virtual bool begin(BSONOArchiver& oar) {
		if (obj_.isEmpty()) return false;

		oar.iterator_ = obj_.begin();

		oar.element_ = oar.iterator_.next();
		if (oar.element_.type() == 3) {
			oar.obj_ = oar.element_.Obj();
			oar.obj_.getOwned();
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
			obj_.getOwned();
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

}}}}


namespace zertcore { namespace db { namespace mongo { namespace serialization {

/**
 * BSONIArchiver
 */
class BSONIArchiver:
		public IArchiver<BSONIArchiver>
{
public:
	struct Cell
	{
		enum {
			OBJ, LIST
		};
		Cell() : type(OBJ) {};

		u32						type;
		BSONObjBuilder			obj;
		BSONArrayBuilder		array;
	};
	typedef map<key_type, BSONIArchiver>	archiver_map_type;

public:
	BSONIArchiver() : label_(""), has_label_(false) {}
	BSONIArchiver(const BSONIArchiver&) : label_(""), has_label_(false) {}
	virtual ~BSONIArchiver() {}

public:
	BSONObj data() const {
		BSONObjBuilder ob;
		if (cell_.type == Cell::OBJ) {
			ob.appendElements(cell_.obj.obj());
		}
		else {
			ob.appendElements(cell_.array.obj());
		}

		for (archiver_map_type::const_iterator it = archiver_map_.begin();
				it != archiver_map_.end(); ++it) {
			ob.append(it->first, it->second.data());
		}

		return ob.obj();
	}

public:
	/**
	 * Mainly for Query commands such as NE LT LTE
	 */
	BSONIArchiver& operator << (const Labeler::Label &l) {
		if (cell_.type != Cell::LIST) {
			return *this;
		}

		label_ = l;
		has_label_ = true;

		return *this;
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
	virtual BSONIArchiver& array() {
		if (key_.empty())
			return *this;

		BSONIArchiver& ar(archiver_map_[key_]);
		ar.setList();
		return ar;
	}
	virtual BSONIArchiver& object() {
		if (key_.empty())
			return *this;

		BSONIArchiver& ar = archiver_map_[key_];
		ar.setObject();
		return ar;
	}

public:
	const Labeler::Label& getLabel() const {
		return label_;
	}
	bool& hasLabel() {
		return has_label_;
	}

private:
	Labeler::Label				label_;
	bool						has_label_;

private:
	mutable Cell				cell_;
	archiver_map_type			archiver_map_;
};

}}}}

namespace zertcore { namespace serialization {
using namespace zertcore::db::mongo;
using namespace zertcore::db::mongo::serialization;

template <>
inline bool operator >> (BSONOArchiver& archiver, i32& value) {
	value = 0;
	BSONElement& elemt = archiver.getElement();

	if (!elemt.ok()) {
		return false;
	}
	if (elemt.type() != NumberInt) {
		try {
			switch (elemt.type()) {
			case NumberDouble:
				value = (i32)elemt.Double();
				return true;
			case String:
				value = lexical_cast<i32>(elemt.String());
				return true;
			case NumberLong:
				value = i32(elemt.Long());
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
inline bool operator >> (BSONOArchiver& archiver, u32& value) {
	value = 0;
	BSONElement& elemt = archiver.getElement();

	if (!elemt.ok()) {
		return false;
	}
	if (elemt.type() != NumberInt) {
		try {
			switch (elemt.type()) {
			case NumberDouble:
				value = (u32)elemt.Double();
				return true;
			case String:
				value = lexical_cast<u32>(elemt.String());
				return true;
			case NumberLong:
				value = u32(elemt.Long());
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
	value = (u32)elemt.Int();

	return true;
}

template <>
inline bool operator >> (BSONOArchiver& archiver, i64& value) {
	value = 0;
	BSONElement& elemt = archiver.getElement();

	if (!elemt.ok()) {
		return false;
	}
	if (elemt.type() != NumberLong) {
		try {
			switch (elemt.type()) {
			case NumberDouble:
				value = (i64)elemt.Double();
				return true;
			case String:
				value = lexical_cast<i64>(elemt.String());
				return true;
			case NumberInt:
				value = i64(elemt.Long());
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
	value = elemt.Long();

	return true;
}

template <>
inline bool operator >> (BSONOArchiver& archiver, u64& value) {
	value = 0;
	BSONElement& elemt = archiver.getElement();

	if (!elemt.ok()) {
		return false;
	}
	if (elemt.type() != NumberLong) {
		try {
			switch (elemt.type()) {
			case NumberDouble:
				value = (u64)elemt.Double();
				return true;
			case String:
				value = lexical_cast<u64>(elemt.String());
				return true;
			case NumberInt:
				value = u64(elemt.Long());
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
	value = (u64)elemt.Long();

	return true;
}

template <>
inline bool operator >> (BSONOArchiver& archiver, string& value) {
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

template <>
inline bool operator >> (BSONOArchiver& archiver, f32& value) {
	BSONElement& elemt = archiver.getElement();

	if (!elemt.ok()) {
		return false;
	}
	if (elemt.type() != NumberDouble) {
		try {
			switch (elemt.type()) {
			case String:
				value = lexical_cast<f32>(elemt.String());
				return true;
			case NumberInt:
				value = lexical_cast<f32>(elemt.Int());
				return true;
			case NumberLong:
				value = lexical_cast<f32>(elemt.Long());
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
	value = (f32)elemt.Double();

	return true;
}

template <>
inline bool operator >> (BSONOArchiver& archiver, f64& value) {
	BSONElement& elemt = archiver.getElement();

	if (!elemt.ok()) {
		return false;
	}
	if (elemt.type() != NumberDouble) {
		try {
			switch (elemt.type()) {
			case String:
				value = lexical_cast<f64>(elemt.String());
				return true;
			case NumberInt:
				value = lexical_cast<f64>(elemt.Int());
				return true;
			case NumberLong:
				value = lexical_cast<f64>(elemt.Long());
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
	value = (f64)elemt.Double();

	return true;
}

}}

namespace zertcore { namespace serialization {
using namespace zertcore::db::mongo::serialization;

template <typename T>
inline BSONIArchiver& operator << (BSONIArchiver& archiver, const T& value) {
	if (archiver.getCell().type == BSONIArchiver::Cell::LIST) {
		archiver.getCell().array.append(value);
	}
	else {
		if (archiver.hasLabel()) {
			archiver.hasLabel() = false;
			archiver.getCell().obj.operator <<(archiver.getKey()).
					operator <<(archiver.getLabel()).operator <<(value);
		}
		else {
			archiver.getCell().obj.append(archiver.getKey(), value);
		}
	}
	return archiver;
}

template <>
inline BSONIArchiver& operator << (BSONIArchiver& archiver, const u64& value) {
	if (archiver.getCell().type == BSONIArchiver::Cell::LIST) {
		archiver.getCell().array.append((long long)value);
	}
	else {
		if (archiver.hasLabel()) {
			archiver.hasLabel() = false;
			archiver.getCell().obj.operator <<(archiver.getKey()).
					operator <<(archiver.getLabel()).operator <<((long long)value);
		}
		else {
			archiver.getCell().obj.append(archiver.getKey(), (long long)value);
		}
	}
	return archiver;
}

/**
template <>
inline BSONIArchiver& operator << (BSONIArchiver& archiver, const time_type& value) {
	if (archiver.getCell().type == BSONIArchiver::Cell::LIST) {
		archiver.getCell().array.append(value);
	}
	else {
		if (archiver.hasLabel()) {
			archiver.hasLabel() = false;
			archiver.getCell().obj.operator <<(archiver.getKey()).
					operator <<(archiver.getLabel()).operator <<(value.value);
		}
		else {
			archiver.getCell().obj.append(archiver.getKey(), value.value);
		}
	}
	return archiver;
}
*/

}}

#endif /* BSONFORMAT_H_ */
