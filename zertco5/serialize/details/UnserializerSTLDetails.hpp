/*
 * UnserializerSTLDetails.hpp
 *
 *  Created on: 2015��1��19��
 *      Author: Administrator
 */

#ifndef ZERTCORE_UNSERIALIZERSTLDETAILS_HPP_
#define ZERTCORE_UNSERIALIZERSTLDETAILS_HPP_

#include "../Unserializer_fwd.h"

namespace zertcore { namespace serialization {

template <class Stream, typename ValueType, class Alloc>
inline bool operator >> (const Unserializer<Stream>& s, list<ValueType, Alloc>& value) {
	typedef typename Unserializer<Stream>::iterator_type
											iterator_type;
	Unserializer<Stream> st;
	if (!s.getValue(st)) {
		return false;
	}

	for (iterator_type it = st.begin(); ; ) {
		ValueType v;
		if (!(st.setIterator(&it) >> v)) {
			break;
		}

		value.push_back(v);
	}

	return !value.empty();
}

template <class Stream, typename ValueType, class Alloc>
inline bool operator >> (const Unserializer<Stream>& s, vector<ValueType, Alloc>& value) {
	typedef typename Unserializer<Stream>::iterator_type
											iterator_type;
	Unserializer<Stream> st;
	if (!s.getValue(st)) {
		return false;
	}

	for (iterator_type it = st.begin(); ; ) {
		ValueType v;
		if (!(st.setIterator(&it) >> v)) {
			break;
		}

		value.push_back(v);
	}

	return !value.empty();
}

template <class Stream, typename ValueType, class Alloc>
inline bool operator >> (const Unserializer<Stream>& s, deque<ValueType, Alloc>& value) {
	typedef typename Unserializer<Stream>::iterator_type
											iterator_type;
	Unserializer<Stream> st;
	if (!s.getValue(st)) {
		return false;
	}

	for (iterator_type it = st.begin(); ; ) {
		ValueType v;
		if (!(st.setIterator(&it) >> v)) {
			break;
		}

		value.push_back(v);
	}

	return !value.empty();
}

template <class Stream, typename ValueType, class Compare, class Alloc>
inline bool operator >> (const Unserializer<Stream>& s, set<ValueType, Compare, Alloc>& value) {
	typedef typename Unserializer<Stream>::iterator_type
											iterator_type;
	Unserializer<Stream> st;
	if (!s.getValue(st)) {
		return false;
	}

	for (iterator_type it = st.begin(); ; ) {
		ValueType v;
		if (!(st.setIterator(&it) >> v)) {
			break;
		}

		value.insert(v);
	}

	return !value.empty();
}

template <class Stream, typename ValueType, class Compare, class Alloc>
inline bool operator >> (const Unserializer<Stream>& s, multiset<ValueType, Compare, Alloc>& value) {
	typedef typename Unserializer<Stream>::iterator_type
											iterator_type;
	Unserializer<Stream> st;
	if (!s.getValue(st)) {
		return false;
	}

	for (iterator_type it = st.begin(); ; ) {
		ValueType v;
		if (!(st.setIterator(&it) >> v)) {
			break;
		}

		value.insert(v);
	}

	return !value.empty();
}

template <class Stream, typename ValueType, class Compare, class Alloc>
inline bool operator >> (const Unserializer<Stream>& s, unordered_set<ValueType, Compare, Alloc>& value) {
	typedef typename Unserializer<Stream>::iterator_type
											iterator_type;
	Unserializer<Stream> st;
	if (!s.getValue(st)) {
		return false;
	}

	for (iterator_type it = st.begin(); ; ) {
		ValueType v;
		if (!(st.setIterator(&it) >> v)) {
			break;
		}

		value.insert(v);
	}

	return !value.empty();
}

template <class Stream, typename ValueType, class Compare, class Alloc>
inline bool operator >> (const Unserializer<Stream>& s, unordered_multiset<ValueType, Compare, Alloc>& value) {
	typedef typename Unserializer<Stream>::iterator_type
											iterator_type;
	Unserializer<Stream> st;
	if (!s.getValue(st)) {
		return false;
	}

	for (iterator_type it = st.begin(); ; ) {
		ValueType v;
		if (!(st.setIterator(&it) >> v)) {
			break;
		}

		value.insert(v);
	}

	return !value.empty();
}

template <class Stream, typename T, typename ValueType, class Compare, class Alloc>
inline bool operator >> (const Unserializer<Stream>& s, map<T, ValueType, Compare, Alloc>& value) {
	typedef map<T, ValueType, Compare, Alloc>
											map_type;
	typedef typename Unserializer<Stream>::iterator_type
											iterator_type;
	Unserializer<Stream> st;
	if (!s.getValue(st)) {
		return false;
	}

	for (iterator_type it = st.begin(); ; ) {
		ValueType v;
		if (!(st.setIterator(&it) >> v)) {
			break;
		}

		try {
			value.insert(typename map_type::value_type(lexical_cast<T>(st.getKey()), v));
		}
		catch(const bad_lexical_cast&) {
			return false;
		}
	}

	return !value.empty();
}

template <class Stream, typename T, typename ValueType, class Compare, class Alloc>
inline bool operator >> (const Unserializer<Stream>& s, unordered_map<T, ValueType, Compare, Alloc>& value) {
	typedef unordered_map<T, ValueType, Compare, Alloc>
											map_type;
	typedef typename Unserializer<Stream>::iterator_type
											iterator_type;
	Unserializer<Stream> st;
	if (!s.getValue(st)) {
		return false;
	}

	for (iterator_type it = st.begin(); ; ) {
		ValueType v;
		if (!(st.setIterator(&it) >> v)) {
			break;
		}

		try {
			value.insert(typename map_type::value_type(lexical_cast<T>(st.getKey()), v));
		}
		catch(const bad_lexical_cast&) {
			return false;
		}
	}

	return !value.empty();
}

}}

#endif /* UNSERIALIZERSTLDETAILS_HPP_ */
