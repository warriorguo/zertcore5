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
	if (s.getIgnoreNull() && v.empty())
		return s;

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
	if (s.getIgnoreNull() && v && !v[0])
		return s;

	s.setValue(v);
	return s;
}

template <class Stream, class F>
inline Serializer<Stream>& operator << (Serializer<Stream>& s, const Serializable<F>& v) {
	Serializer<Stream> st(TYPE_OBJECT, s.stream(), s.getIgnoreNull());
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
template <typename T>
Serializer<Stream>::Serializer(const T& v, bool ignore_null) {
	archiver_ = archiver_type::create(TYPE_OBJECT);
	archiver_->stream().setType(TYPE_OBJECT);
	if (ignore_null) setIgnoreNull();

	(*this) << v;
}

template <class Stream>
Serializer<Stream>::Serializer(bool ignore_null) {
	archiver_ = archiver_type::create(TYPE_OBJECT);
	archiver_->stream().setType(TYPE_OBJECT);
	if (ignore_null) setIgnoreNull();
}

template <class Stream>
Serializer<Stream>::Serializer(const value_type& type, bool ignore_null) {
	archiver_ = archiver_type::create(type);
	archiver_->stream().setType(type);
	if (ignore_null) setIgnoreNull();
}

template <class Stream>
Serializer<Stream>::Serializer(const value_type& type, stream_type& stream, bool ignore_null) {
	archiver_ = archiver_type::create(type, stream);
	archiver_->stream().setType(type);
	if (ignore_null) setIgnoreNull();
}

template <class Stream>
Serializer<Stream>::Serializer(const self_type& ar, bool ignore_null) : archiver_(ar.archiver_) {
	if (ignore_null) setIgnoreNull();
}

template <class Stream>
Serializer<Stream>::Serializer(self_type& ar, bool ignore_null) : archiver_(ar.archiver_) {
	if (ignore_null) setIgnoreNull();
}

template <class Stream>
typename Serializer<Stream>::self_type& Serializer<Stream>::operator= (const self_type& ar) {
	archiver_ = ar.archiver_;
	return *this;
}

template <class Stream>
void Serializer<Stream>::
addKey(const key_type& key) {
	archiver_->addKey(key);
}

template <class Stream>
key_list_type& Serializer<Stream>::
getKeys() const {
	return archiver_->keyList();
}

template <class Stream>
bool Serializer<Stream>::
hasKey() const {
	return !isEmpty(archiver_->keyList());
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
	else if (!hasKey()) {
		archiver_->stream().addObject(key_type(), v);
	}
	else {
		if (archiver_->keySize() == 1)
			archiver_->stream().addObject(archiver_->lastKey(), v);
		else {
			self_type c(getIgnoreNull());
			c.archiver_->stream().addObject(archiver_->lastKey(), v);

			const key_list_type& list = archiver_->keyList();
			key_list_type::const_reverse_iterator it = list.rbegin(); ++it;
			key_list_type::const_reverse_iterator end_it = list.rend(); --end_it;

			for (; it != end_it; ++it) {
				self_type cp(getIgnoreNull());
				cp.archiver_->stream().addObject(*it, c.archiver_->stream().data());
				c = cp;
			}

			archiver_->stream().addObject(*it, c.archiver_->stream().data());
		}
	}

	archiver_->clearKeys();
}

template <class Stream>
void Serializer<Stream>::
setValue(const char* v) {
	if (archiver_->getType() == TYPE_ARRAY) {
		archiver_->stream().addList(v);
	}
	else if (!hasKey()) {
		archiver_->stream().addObject(key_type(), v);
	}
	else {
		if (archiver_->keySize() == 1)
			archiver_->stream().addObject(archiver_->lastKey(), v);
		else {
			self_type c(getIgnoreNull());
			c.archiver_->stream().addObject(archiver_->lastKey(), v);

			const key_list_type& list = archiver_->keyList();
			key_list_type::const_reverse_iterator it = list.rbegin(); ++it;
			key_list_type::const_reverse_iterator end_it = list.rend(); --end_it;

			for (; it != end_it; ++it) {
				self_type cp(getIgnoreNull());
				cp.archiver_->stream().addObject(*it, c.archiver_->stream().data());
				c = cp;
			}

			archiver_->stream().addObject(*it, c.archiver_->stream().data());
		}
	}

	archiver_->clearKeys();
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
	else if (!hasKey()) {
		archiver_->stream().combine(v.archiver_->stream());
	}
	else {
		if (archiver_->keySize() == 1)
			archiver_->stream().addObject(archiver_->lastKey(), v.archiver_->stream().data());
		else {
			self_type c(getIgnoreNull());
			c.archiver_->stream().addObject(archiver_->lastKey(), v.archiver_->stream().data());

			const key_list_type& list = archiver_->keyList();
			key_list_type::const_reverse_iterator it = list.rbegin(); ++it;
			key_list_type::const_reverse_iterator end_it = list.rend(); --end_it;

			for (; it != end_it; ++it) {
				self_type cp(getIgnoreNull());
				cp.archiver_->stream().addObject(*it, c.archiver_->stream().data());
				c = cp;
			}

			archiver_->stream().addObject(*it, c.archiver_->stream().data());
		}
	}

	archiver_->clearKeys();
}

}}

#include "details/SerializerSTLDetails.hpp"

#endif /* SERIALIZERDETAILS_HPP_ */
