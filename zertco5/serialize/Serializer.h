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
inline Serializer<Stream>& operator << (Serializer<Stream>& s, const string& v) {
	s.setValue(v);
	return s;
}

template <class Stream>
inline Serializer<Stream>& operator << (Serializer<Stream>& s, const Serializer<Stream>& v) {
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
	Serializer<Stream> st(TYPE_OBJECT, s.stream());
	const_cast<F &>(static_cast<F const &>(v)).serialize(st);

	s.setValue(st);
	return s;
}

#ifdef ZC_COMPILE
#  include "details/SerializerSimpleTypeOperatorDetails.ipp"
#else

template <class Stream, class T>
inline Serializer<Stream>& operator << (Serializer<Stream>& s, const T& v) {
	s.setValue(v);
	return s;
}

#endif

}}

namespace zertcore { namespace serialization {

template <class Stream>
Serializer<Stream>::Serializer() {
	archiver_ = archiver_type::create(TYPE_OBJECT);
	archiver_->stream().setType(TYPE_OBJECT);
}

template <class Stream>
Serializer<Stream>::Serializer(const value_type& type) {
	archiver_ = archiver_type::create(type);
	archiver_->stream().setType(type);
}

template <class Stream>
Serializer<Stream>::Serializer(const value_type& type, stream_type& stream) {
	archiver_ = archiver_type::create(type, stream);
	archiver_->stream().setType(type);
}

template <class Stream>
Serializer<Stream>::Serializer(const self_type& ar) : archiver_(ar.archiver_) {;}

template <class Stream>
Serializer<Stream>::Serializer(self_type& ar) : archiver_(ar.archiver_) {;}

template <class Stream>
typename Serializer<Stream>::self_type& Serializer<Stream>::operator= (const self_type& ar) {
	archiver_ = ar.archiver_;
	return *this;
}

template <class Stream>
void Serializer<Stream>::
setKey(const key_type& key) {
	archiver_->key() = key;
}

template <class Stream>
template <typename T>
typename Serializer<Stream>::self_type& Serializer<Stream>::
operator& (const T& v) {
	return (*this) << v;
}

template <class Stream>
typename Serializer<Stream>::self_type& Serializer<Stream>::
operator& (const char *v) {
	return (*this) << v;
}

template <class Stream>
void Serializer<Stream>::
setSize(const size_t& size) {
	if (archiver_->getType() == TYPE_ARRAY) {
		archiver_->stream().setListSize(size);
	}
	else {
		archiver_->stream().setObjectSize(size);
	}
}

template <class Stream>
SharedBuffer Serializer<Stream>::
buffer() const {
	return archiver_->stream().buffer();
}

template <class Stream>
template <typename T>
void Serializer<Stream>::
setValue(const T& v) {
	if (archiver_->getType() == TYPE_ARRAY) {
		archiver_->stream().addList(v);
	}
	else {
		archiver_->stream().addObject(archiver_->key(), v);
	}
}

template <class Stream>
void Serializer<Stream>::
setValue(const char* v) {
	if (archiver_->getType() == TYPE_ARRAY) {
		archiver_->stream().addList(v);
	}
	else {
		archiver_->stream().addObject(archiver_->key(), v);
	}
}

template <class Stream>
void Serializer<Stream>::
setValue(const self_type& v) {
	/**
	 * add the child to forbid the inter data of archiver releasing
	 */
	archiver_->addChild(v.archiver_);

	if (archiver_->getType() == TYPE_ARRAY) {
		archiver_->stream().addList(v.archiver_->stream().data());
	}
	else {
		archiver_->stream().addObject(archiver_->key(), v.archiver_->stream().data());
	}
}

}}

#include "details/SerializerSTLDetails.hpp"

#endif /* SERIALIZERDETAILS_HPP_ */
