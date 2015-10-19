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
	if (!s.getValue(st)) {
//		ZCLOG(WARNING) << "Unserializer<Stream>.getValue() failed" << End;
		return false;
	}

	return static_cast<F &>(v).unserialize(st);
}

template <class Stream>
inline bool operator >> (const Unserializer<Stream>& s, Unserializer<Stream>& v) {
	return s.getValue(v);
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
Unserializer<Stream>::Unserializer(const self_type& ar) : archiver_(ar.archiver_),
	from_type_(ar.from_type_), p_iterator_(ar.p_iterator_) {;}

template <class Stream>
Unserializer<Stream>::Unserializer(self_type& ar) : archiver_(ar.archiver_),
	from_type_(ar.from_type_), p_iterator_(ar.p_iterator_) {;}

template <class Stream>
template <class T>
Unserializer<Stream>::Unserializer(const Serializer<T>& c) :
	from_type_(FROM_NONE), p_iterator_(NULL) {
	archiver_ = archiver_type::create(TYPE_NONE);
	archiver_->stream().data(c.stream().data());
}

template <class Stream>
typename Unserializer<Stream>::self_type& Unserializer<Stream>::operator= (const self_type& ar) {
	archiver_ = ar.archiver_;
	return *this;
}

template <class Stream>
void Unserializer<Stream>::
addKey(const key_type& key) const {
	archiver_->addKey(key);
	from_type_ = FROM_KEY;
}

template <class Stream>
void Unserializer<Stream>::
setKeys(const key_list_type& keys) const {
	archiver_->keyList() = keys;
	from_type_ = FROM_KEY;
}

template <class Stream>
const key_list_type& Unserializer<Stream>::
getKeys() const {
	return archiver_->keyList();
}

template <class Stream>
const key_type& Unserializer<Stream>::
lastKey() const {
	return archiver_->lastKey();
}

template <class Stream>
bool Unserializer<Stream>::
hasKey() const {
	return !isEmpty(archiver_->key());
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
T Unserializer<Stream>::
get(const key_type& key, const T& default_value) const {
	T tmp(default_value);
	(*this)[key] & tmp;

	return tmp;
}

template <class Stream>
template <typename T>
bool Unserializer<Stream>::
getValue(T& v) const {
	bool ret = false;
	if (from_type_ == FROM_KEY) {

		if (archiver_->keySize() == 1) {
			ret = archiver_->stream().getValue(archiver_->lastKey(), v);
		}
		else {
			const key_list_type& list = archiver_->keyList();
			key_list_type::const_iterator end_it = list.end(); --end_it;
			key_list_type::const_iterator it = list.begin();

			self_type c;
			ret = archiver_->stream().getValue(*it, c.archiver_->stream()); ++it;

			if (ret) {
				for (; it != end_it; ++it) {
					self_type ch;
					ret = c.archiver_->stream().getValue(*it, ch.archiver_->stream());
					if (!ret) break;
					c = ch;
				}

				if (ret)
					ret = c.archiver_->stream().getValue(*it, v);
			}

		}

	}
	else if (from_type_ == FROM_ITERATOR)
		ret = archiver_->stream().getValue(*p_iterator_, read_key_, v);
	else
		ZC_ASSERT(false);

	archiver_->clearKeys();
	from_type_ = FROM_NONE;
	return ret;
}

template <class Stream>
bool Unserializer<Stream>::
getValue(self_type& v) const {
	bool ret = false;
	if (from_type_ == FROM_KEY) {

		if (archiver_->keySize() == 1) {
			ret = archiver_->stream().getValue(archiver_->lastKey(), v.archiver_->stream());
		}
		else {
			const key_list_type& list = archiver_->keyList();
			key_list_type::const_iterator end_it = list.end(); --end_it;
			key_list_type::const_iterator it = list.begin();

			self_type c;
			ret = archiver_->stream().getValue(*it, c.archiver_->stream()); ++it;

			if (ret) {
				for (; it != end_it; ++it) {
					self_type ch;
					ret = c.archiver_->stream().getValue(*it, ch.archiver_->stream());
					if (!ret) break;
					c = ch;
				}

				if (ret)
					ret = c.archiver_->stream().getValue(*it, v.archiver_->stream());
			}

		}

	}
	else if (from_type_ == FROM_ITERATOR)
		ret = archiver_->stream().getValue(*p_iterator_, read_key_, v.archiver_->stream());
	else {
		v.archiver_ = archiver_;
		ret = true;
	}

	archiver_->clearKeys();
	from_type_ = FROM_NONE;
	return ret;
}

template <class Stream>
template <typename T>
bool Unserializer<Stream>::
getValue(iterator_type* iter, T& v) const {
	return archiver_->stream().getValue(*iter, read_key_, v);
}

template <class Stream>
bool Unserializer<Stream>::
getValue(iterator_type* iter, self_type& v) const {
	return archiver_->stream().getValue(*iter, read_key_, v.archiver_->stream());
}

/**
template <class Stream>
value_type Unserializer<Stream>::
getType() const {
	if (from_type_ == FROM_KEY)
		return archiver_->stream().getType(archiver_->key());
	else if (from_type_ == FROM_ITERATOR)
		return archiver_->stream().getType(*p_iterator_);
	return TYPE_NONE;
}
*/

template <class Stream>
value_type Unserializer<Stream>::
getElementType() const {
	if (from_type_ == FROM_KEY) {
		if (archiver_->keySize() == 1) {
			return archiver_->stream().getType(archiver_->lastKey());
		}
		else {
			self_type c;
			const key_list_type& list = archiver_->keyList();
			key_list_type::const_iterator end_it = list.end(); --end_it;
			key_list_type::const_iterator it = list.begin();

			bool ret = archiver_->stream().getValue(*it, c.archiver_->stream()); ++it;

			if (ret) {
				for (; it != end_it; ++it) {
					self_type ch;
					ret = c.archiver_->stream().getValue(*it, ch.archiver_->stream());
					if (!ret) break;
					c = ch;
				}

				if (ret)
					return archiver_->stream().getType(*it);
			}

			return TYPE_NONE;
		}

	}
	else if (from_type_ == FROM_ITERATOR)
		return archiver_->stream().getType(*p_iterator_);
	return TYPE_NONE;
}

template <class Stream>
typename Unserializer<Stream>::iterator_type Unserializer<Stream>::
begin() const {
	return archiver_->stream().begin();
}

template <class Stream>
bool Unserializer<Stream>::
buffer(const SharedBuffer& buf) {
	return archiver_->stream().buffer(buf);
}

}}

#include "details/UnserializerSTLDetails.hpp"

#endif /* UNSERIALIZERDETAILS_HPP_ */
