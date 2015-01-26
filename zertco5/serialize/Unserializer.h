/*
 * Unserializer.h
 *
 *  Created on: 2015Äê1ÔÂ19ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_UNSERIALIZER_H_
#define ZERTCORE_UNSERIALIZER_H_

#include <pch.h>
#include <utils/types.h>
#include <log/Log.h>
#include <object/PoolObject.h>

#include "config.h"

namespace zertcore { namespace serialization {
using namespace zertcore::utils;
using namespace zertcore::object;
}}

namespace zertcore {

template <class F>
struct Unserializable {};

}

namespace zertcore { namespace serialization {

/**
 * Stream defines:
class Stream
{
public:
	template <typename T>
	bool getValue(key_type& key, T& v);

	bool getStream(key_type& key, Stream& stream);

public:
	void setObject();
	void setArray();

public:
	bool setStr(const string&);
};
*/
template <class Stream>
class Unserializer :
		public PoolObject<Unserializer<Stream> >,
		noncopyable
{
	typedef Unserializer<Stream>			self_type;
public:
	typedef Stream							stream_type;

public:
	typedef typename self_type::ptr			ptr;
	typedef vector<ptr>						auto_save_ptr_list_type;

public:
	explicit Unserializer() : type_(details::TYPE_OBJECT) {
		setupType();
	}
	explicit Unserializer(const details::value_type& type) : type_(type) {
		setupType();
	}

public:
	void setupType() {
		switch(type_) {
		case details::TYPE_OBJECT:
			stream_.setObject();
			break;
		case details::TYPE_ARRAY:
			stream_.setArray();
			break;
		default:
			ZCLOG(FINAL) << "Unspport Types" << End;
			break;
		}
	}

public:
	stream_type& stream() {return stream_;}

public:
	template <typename T>
	bool operator& (T& v) {
		return *this >> v;
	}

	self_type& operator[] (const key_type& key) {
		key_ = key;
		return *this;
	}

public:
	bool getValue(string& v) {
		return stream_.getValue(key_, v);
	}

#ifdef ZC_COMPILE
#  include "details/UnserializerGetValueDetails.ipp"
#else

	template <typename T>
	bool getValue(T& v) {
		return stream_.getValue(key_, v);
	}

#endif

public:
	bool parse(const string& source) {
		return stream_.str(source);
	}

public:
	key_type getKey() const {
		return key_;
	}

public:
	bool getSerializer(self_type& v) {
		return v.stream_.getStream(v.key_, stream_);
	}

	ZC_TO_STRING("key:" << key_ << " type:" << type_)

private:
	stream_type					stream_;

private:
	key_type					key_;
	details::value_type			type_;
};

#ifdef ZC_COMPILE
#  include "details/UnserializerSimpleTypeOperatorDetails.ipp"
#else

template <class Stream, typename T>
inline bool operator >> (Unserializer<Stream>& s, T& v) {
	return s.getValue(v);
}

#endif

template <class Stream>
inline bool operator >> (Unserializer<Stream>& s, string& v) {
	return s.getValue(v);
}

template <class Stream, class F>
inline bool operator >> (Unserializer<Stream>& s, Unserializable<F>& v) {
	Unserializer<Stream> st(details::TYPE_OBJECT);
	if (!st.getSerializer(s))
		return false;

	return static_cast<F &>(v).unserialize(st);
}

}}

#include "details/UnserializerSTLDetails.hpp"
#endif /* UNSERIALIZER_H_ */
