/*
 * Serializer.h
 *
 *  Created on: 2015��1��19��
 *      Author: Administrator
 */

#ifndef ZERTCORE_SERIALIZER_H_
#define ZERTCORE_SERIALIZER_H_

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
struct Serializable {};

}

namespace zertcore { namespace serialization {

/**
 * Stream defines:
class Stream
{
public:
	template <typename T>
	void setValue(const key_type& key, const T& v, op_code_type);

	void setStream(const key_type& key, const Stream&);

public:
	void setObject();
	void setArray();

public:
	string str();
};
*/
template <class Stream>
class Serializer :
		public PoolObject<Serializer<Stream> >,
		noncopyable
{
	typedef Serializer<Stream>				self_type;
public:
	typedef Stream							stream_type;

public:
	typedef typename self_type::ptr			ptr;
	typedef vector<ptr>						auto_save_ptr_list_type;

public:
	explicit Serializer() : type_(details::TYPE_OBJECT), op_code_(OP_NONE) {
		stream_.setObject();
	}
	explicit Serializer(const details::value_type& type) : type_(type), op_code_(OP_NONE) {
		switch(type) {
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

	virtual ~Serializer() {}

public:
	self_type& operator[] (const key_type& key) {
		return setKey(key);
	}

	template <typename T>
	self_type& operator& (const T& v) {
		return *this << v;
	}

public:
	stream_type& stream() {return stream_;}
	const stream_type& stream() const {return stream_;}

public:
	self_type& setKey(const key_type& key) {
		key_ = key;
		return *this;
	}

	self_type& setOpCode(const op_code_type& op_code) {
		op_code_ = op_code;
		return *this;
	}

	op_code_type getOpCode() const {
		return op_code_;
	}

	void setValue(const string& v) {
		stream_.setValue(key_, v, op_code_);
		op_code_ = OP_NONE;
	}

	void setValue(const char* v) {
		stream_.setValue(key_, v, op_code_);
		op_code_ = OP_NONE;
	}

#ifdef ZC_COMPILE
#  include "details/SerializerSetValueDetails.ipp"
#else

	template <typename T>
	void setValue(const T& v) {
		stream_.setValue(key_, v, op_code_);
		op_code_ = OP_NONE;
	}

#endif

	string getString() {
		return stream_.str();
	}

	void setSerializer(self_type& v) {
		stream_.setStream(key_, v.stream_, op_code_);
		op_code_ = OP_NONE;
	}

	void setSerializer(ptr pv) {
		stream_.setStream(key_, pv->stream_, op_code_);
		op_code_ = OP_NONE;

		auto_save_ptr_list_.push_back(pv);
	}

	ZC_TO_STRING("key:" << key_ << " type:" << type_)

private:
	stream_type					stream_;
	auto_save_ptr_list_type		auto_save_ptr_list_;

private:
	key_type					key_;
	details::value_type			type_;

private:
	op_code_type				op_code_;
};

#ifdef ZC_COMPILE
#  include "details/SerializerSimpleTypeOperatorDetails.ipp"
#else

template <class Stream, typename T>
inline Serializer<Stream>& operator << (Serializer<Stream>& s, const T& v) {
	s.setValue(v);
	return s;
}

#endif

template <class Stream>
inline Serializer<Stream>& operator << (Serializer<Stream>& s, const string& v) {
	s.setValue(v);
	return s;
}

template <class Stream>
inline Serializer<Stream>& operator << (Serializer<Stream>& s, const char* v) {
	s.setValue(v);
	return s;
}

template <class Stream, class F>
inline Serializer<Stream>& operator << (Serializer<Stream>& s, const Serializable<F>& v) {
	typename Serializer<Stream>::ptr st = Serializer<Stream>::create(details::TYPE_OBJECT);
	static_cast<F const &>(v).serialize(*st);

	s.setSerializer(st);
	return s;
}

}}

#include "details/SerializerSTLDetails.hpp"
#endif /* SERIALIZER_H_ */
