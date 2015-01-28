/*
 * UnserializerDetails.hpp
 *
 *  Created on: 2015年1月28日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UNSERIALIZERDETAILS_HPP_
#define ZERTCORE_UNSERIALIZERDETAILS_HPP_

#include "Serializer_fwd.h"
#include <log/Log.h>

namespace zertcore { namespace serialization {

template <class Stream>
inline bool operator >> (const Unserializer<Stream>& s, string& v) {
	s.setValue(v);
	return s;
}

template <class Stream>
template <class F>
inline bool operator << (const Unserializer<Stream>& s, Serializable<F>& v) {
	Unserializer<Stream> st();
	s.getValue(st);

	return static_cast<F const &>(v).unserialize(st);
}

}}

namespace zertcore { namespace serialization {

template <class Stream>
Unserializer<Stream>::Unserializer() {
	archiver_ = archiver_type::create(TYPE_NONE);
}

template <class Stream>
void Unserializer<Stream>::
setKey(const key_type& key) const {
	key_ = key;
}

template <class Stream>
template <typename T>
typename Unserializer<Stream>::self_type& Unserializer<Stream>::
operator& (T& v) {
	return (*this) >> v;
}

template <class Stream>
void Unserializer<Stream>::
setKey(const key_type& key) const {
	archiver_->setKey(key);
}

template <class Stream>
template <typename T>
bool Unserializer<Stream>::
getValue(T& v) const {
	return archiver_->stream().getValue(archiver_->key(), v);
}

template <class Stream>
bool Unserializer<Stream>::
getValue(self_type& v) const {
	return archiver_->stream().getValue(archiver_->key(), v.archiver_->stream().data());
}

template <class Stream>
template <typename T>
bool Unserializer<Stream>::
getValue(iterator_type& iter, T& v) const {
	return archiver_->stream().getValue(iter, archiver_->key(), v);
}

template <class Stream>
bool Unserializer<Stream>::
getValue(iterator_type& iter, self_type& v) const {
	return archiver_->stream().getValue(iter, archiver_->key(), v.archiver_->stream().data());
}

template <class Stream>
value_type Unserializer<Stream>::
getType() const {
	return archiver_->stream().getType();
}

template <class Stream>
Unserializer<Stream>::iterator_type Unserializer<Stream>::
begin() const {
	return archiver_->stream().begin();
}

template <class Stream>
Unserializer<Stream>::iterator_type Unserializer<Stream>::
end() const {
	return archiver_->stream().end();
}

}}


#endif /* UNSERIALIZERDETAILS_HPP_ */
