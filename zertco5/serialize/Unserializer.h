/*
 * UnserializerDetails.hpp
 *
 *  Created on: 2015年1月28日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UNSERIALIZERDETAILS_HPP_
#define ZERTCORE_UNSERIALIZERDETAILS_HPP_

#include "Unserializer_fwd.h"
#include <log/Log.h>

namespace zertcore { namespace serialization {

template <class Stream>
inline bool operator >> (const Unserializer<Stream>& s, string& v) {
	return s.getValue(v);
}

template <class Stream, class F>
inline bool operator >> (const Unserializer<Stream>& s, Unserializable<F>& v) {
	Unserializer<Stream> st;
	s.getValue(st);

	return static_cast<F &>(v).unserialize(st);
}

#ifdef ZC_COMPILE
#  include "details/UnserializerSimpleTypeOperatorDetails.ipp"
#else

template <class Stream, class T>
inline bool operator >> (const Unserializer<Stream>& s, T& v) {
	return s.getValue(v);
}

#endif

}}

namespace zertcore { namespace serialization {

template <class Stream>
Unserializer<Stream>::Unserializer() : from_type_(FROM_NONE), p_iterator_(NULL) {
	archiver_ = archiver_type::create(TYPE_NONE);
}

template <class Stream>
void Unserializer<Stream>::
setKey(const key_type& key) const {
	archiver_->key() = key;
	from_type_ = FROM_KEY;
}

template <class Stream>
const key_type& Unserializer<Stream>::
getKey() const {
	return archiver_->key();
}

template <class Stream>
const typename Unserializer<Stream>::self_type& Unserializer<Stream>::
setIterator(iterator_type* iter) const {
	p_iterator_ = iter;
	from_type_ = FROM_ITERATOR;
	return *this;
}

template <class Stream>
template <typename T>
bool Unserializer<Stream>::
operator& (T& v) const {
	return (*this) >> v;
}

template <class Stream>
template <typename T>
bool Unserializer<Stream>::
getValue(T& v) const {
	bool ret = false;
	if (from_type_ == FROM_KEY)
		ret = archiver_->stream().getValue(archiver_->key(), v);
	else if (from_type_ == FROM_ITERATOR)
		ret = archiver_->stream().getValue(*p_iterator_, archiver_->key(), v);
	else
		ZC_ASSERT(false);

	from_type_ = FROM_NONE;
	return ret;
}

template <class Stream>
bool Unserializer<Stream>::
getValue(self_type& v) const {
	bool ret = false;
	if (from_type_ == FROM_KEY)
		ret = archiver_->stream().getValue(archiver_->key(), v.archiver_->stream().data())
				&& v.archiver_->stream().initData();
	else if (from_type_ == FROM_ITERATOR)
		ret = archiver_->stream().getValue(*p_iterator_, archiver_->key(), v.archiver_->stream().data())
				&& v.archiver_->stream().initData();
	else
		ZC_ASSERT(false);

	from_type_ = FROM_NONE;
	return ret;
}

template <class Stream>
template <typename T>
bool Unserializer<Stream>::
getValue(iterator_type* iter, T& v) const {
	return archiver_->stream().getValue(*iter, archiver_->key(), v);
}

template <class Stream>
bool Unserializer<Stream>::
getValue(iterator_type* iter, self_type& v) const {
	return archiver_->stream().getValue(*iter, archiver_->key(), v.archiver_->stream().data())
			&& v.archiver_->stream().initData();
}

template <class Stream>
value_type Unserializer<Stream>::
getType() const {
	return archiver_->stream().getType();
}

template <class Stream>
typename Unserializer<Stream>::iterator_type Unserializer<Stream>::
begin() const {
	return archiver_->stream().begin();
}

}}

#include "details/UnserializerSTLDetails.hpp"

#endif /* UNSERIALIZERDETAILS_HPP_ */
