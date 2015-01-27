/*
 * SerializerDetails.hpp
 *
 *  Created on: 2015年1月27日
 *      Author: Administrator
 */

#ifndef ZERTCORE_SERIALIZERDETAILS_HPP_
#define ZERTCORE_SERIALIZERDETAILS_HPP_

#include "Serializer_fwd.h"
#include <log/Log.h>

namespace zertcore { namespace serialization {

template <class Stream>
inline Serializer<Stream>& operator << (typename Serializer<Stream>::ptr s, const string& v) {
	s->setValue(v);
	return s;
}

template <class Stream>
inline Serializer<Stream>& operator << (typename Serializer<Stream>::ptr s, const char* v) {
	s->setValue(v);
	return s;
}

template <class Stream, class F>
inline Serializer<Stream>& operator << (typename Serializer<Stream>::ptr s, const Serializable<F>& v) {
	typename Serializer<Stream>::ptr st = Serializer<Stream>::create(TYPE_OBJECT);
	static_cast<F const &>(v).serialize(st);

	s->setValue(st);
	return s;
}

template <class Stream>
template <typename T>
inline Serializer<Stream>& operator & (typename Serializer<Stream>::ptr s, const T& v) {
	return s << v;
}

template <class Stream>
inline Serializer<Stream>& operator & (typename Serializer<Stream>::ptr s, const char* v) {
	return s << v;
}

}}

namespace zertcore { namespace serialization {

template <class Stream>
Serializer<Stream>::Serializer() : type_(TYPE_OBJECT) {
	stream_.setType(type_);
}

template <class Stream>
Serializer<Stream>::Serializer(const value_type& type) : type_(type) {
	stream_.setType(type_);
}

template <class Stream>
void Serializer<Stream>::
setKey(const key_type& key) {
	key_ = key;
}

template <class Stream>
void Serializer<Stream>::
setSize(const size_t& size) {
	stream_.setSize(size);
}

template <class Stream>
template <typename T>
void Serializer<Stream>::
setValue(const T& v) {
	if (type_ == TYPE_ARRAY) {
		stream_.addList(v);
	}
	else {
		stream_.addObject(key_, v);
	}
}

template <class Stream>
void Serializer<Stream>::
setValue(const char* v) {
	if (type_ == TYPE_ARRAY) {
		stream_.addList(v);
	}
	else {
		stream_.addObject(key_, v);
	}
}

template <class Stream>
void Serializer<Stream>::
setValue(ptr v) {
	if (type_ == TYPE_ARRAY) {
		stream_.addList(v->data());
	}
	else {
		stream_.addObject(key_, v->data());
	}
}

}}


#endif /* SERIALIZERDETAILS_HPP_ */
