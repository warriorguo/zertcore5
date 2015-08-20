/*
 * Serializable.h
 *
 *  Created on: 2015年6月19日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_SERIALIZABLEOBJECT_H_
#define ZERTCORE_UTILS_SERIALIZABLEOBJECT_H_

#include <pch.h>
#include <utils/types.h>

#include <serialize/Serializer_fwd.h>
#include <serialize/Unserializer_fwd.h>


namespace zertcore { namespace utils {
struct SerializableObject;
}}

namespace zertcore { namespace serialization {
using namespace zertcore::utils;

template <class Stream>
Serializer<Stream>& operator << (Serializer<Stream>& s, SerializableObject& v);

template <class Stream>
bool operator >> (const Unserializer<Stream>& s, SerializableObject& v);

}}


#include <serialize/Serializer.h>
#include <serialize/Unserializer.h>

#include <concurrent/rpc/config.h>

#include <utils/msgpack/MsgPackStream.h>
#include <db/mongodb/serialize/BSONStream.h>

#include <db/mongodb/details/MongoDBDataProvider.h>

#include "SerializeValues.hpp"

/**
 * Any new provider and serialize way add in
 * provider
 */

namespace zertcore {

enum {
	PROVIDER_MONGODB						= 0,
	PROVIDER_RPC							= 1,
};

}

namespace zertcore { namespace utils {
using namespace zertcore::db;
using namespace zertcore::db::mongodb::serialization;
using namespace zertcore::concurrent;

typedef serialization::Serializer<BSONIStream>
											mongodb_iarchiver_type;
typedef serialization::Unserializer<BSONOStream>
											mongodb_oarchiver_type;

typedef SerializeValuesHost<mongodb_iarchiver_type, mongodb_oarchiver_type>
											mongodb_archiver;
typedef SerializeValuesHost<rpc::iarchiver_type, rpc::oarchiver_type>
											rpc_archiver;
}}

namespace zertcore { namespace utils {

/**
 * SerializableObject
 */
struct SerializableObject :
//		public mongodb_archiver,
		public rpc_archiver
{
	/**
	void serialize(mongodb_iarchiver_type& ar) {
		mongodb_archiver::serialize(ar);
	}
	bool unserialize(const mongodb_oarchiver_type& ar) {
		return mongodb_archiver::unserialize(ar);;
	}
	*/
	void serialize(rpc::iarchiver_type& ar) {
		rpc_archiver::serialize(ar);
	}
	bool unserialize(const rpc::oarchiver_type& ar) {
		return rpc_archiver::unserialize(ar);
	}
};

}}

namespace zertcore { namespace serialization {
using namespace zertcore::utils;

template <class Stream>
inline Serializer<Stream>& operator << (Serializer<Stream>& s, SerializableObject& v) {
	v.serialize(s);
	return s;
}

template <class Stream>
inline bool operator >> (const Unserializer<Stream>& s, SerializableObject& v) {
	return v.unserialize(s);
}

}}

/**
 * For using of ZCVal() be careful about the name duplicate problem!
 * for instance:
 * struct A : SerializableObject
 * {
 * 	ZCVal(i32, a);
 * };
 *
 * struct B : public A
 * {
 *  ZCVal(i32, a);
 * };
 *
 * the variable a's key was "a" was ALWAYS pointing to B::a!
 */
#define ZCVal(type, name)			\
	type name;\
	::zertcore::utils::value::ValueWrapper<type, ::zertcore::utils::mongodb_archiver> \
	 name##_wrapper_{name,*this,#name};

#endif /* UTILS_SERIALIZABLE_H_ */
