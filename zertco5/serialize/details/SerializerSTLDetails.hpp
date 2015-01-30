/*
 * SerializerSTLDetails.hpp
 *
 *  Created on: 2015��1��19��
 *      Author: Administrator
 */

#ifndef ZERTCORE_SERIALIZERSTLDETAILS_HPP_
#define ZERTCORE_SERIALIZERSTLDETAILS_HPP_

#include "../Serializer_fwd.h"

namespace zertcore { namespace serialization {


template <class Stream, typename ValueType, class Alloc>
inline Serializer<Stream>& operator << (Serializer<Stream>& s, const list<ValueType, Alloc>& value) {
	typedef list<ValueType, Alloc>			list_type;

	Serializer<Stream> st(TYPE_ARRAY);
	for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		st << *it;
	}

	s.setValue(st);
	return s;
}

template <class Stream, typename ValueType, class Alloc>
inline Serializer<Stream>& operator << (Serializer<Stream>& s, const vector<ValueType, Alloc>& value) {
	typedef vector<ValueType, Alloc>			list_type;

	Serializer<Stream> st(TYPE_ARRAY);
	for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		st << *it;
	}

	s.setValue(st);
	return s;
}

template <class Stream, typename ValueType, class Alloc>
inline Serializer<Stream>& operator << (Serializer<Stream>& s, const deque<ValueType, Alloc>& value) {
	typedef deque<ValueType, Alloc>			list_type;

	Serializer<Stream> st(TYPE_ARRAY);
	for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		st << *it;
	}

	s.setValue(st);
	return s;
}

template <class Stream, typename ValueType, class Compare, class Alloc>
inline Serializer<Stream>& operator << (Serializer<Stream>& s, const set<ValueType, Compare, Alloc>& value) {
	typedef set<ValueType, Compare, Alloc>	list_type;

	Serializer<Stream> st(TYPE_ARRAY);
	for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		st << *it;
	}

	s.setValue(st);
	return s;
}

template <class Stream, typename ValueType, class Compare, class Alloc>
inline Serializer<Stream>& operator << (Serializer<Stream>& s, const multiset<ValueType, Compare, Alloc>& value) {
	typedef multiset<ValueType, Compare, Alloc>
											list_type;

	Serializer<Stream> st(TYPE_ARRAY);
	for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		st << *it;
	}

	s.setValue(st);
	return s;
}

template <class Stream, typename ValueType, class Compare, class Alloc>
inline Serializer<Stream>& operator << (Serializer<Stream>& s, const unordered_set<ValueType, Compare, Alloc>& value) {
	typedef set<ValueType, Compare, Alloc>	list_type;

	Serializer<Stream> st(TYPE_ARRAY);
	for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		st << *it;
	}

	s.setValue(st);
	return s;
}

template <class Stream, typename ValueType, class Compare, class Alloc>
inline Serializer<Stream>& operator << (Serializer<Stream>& s, const unordered_multiset<ValueType, Compare, Alloc>& value) {
	typedef set<ValueType, Compare, Alloc>	list_type;

	Serializer<Stream> st(TYPE_ARRAY);
	for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		st << *it;
	}

	s.setValue(st);
	return s;
}

template <class Stream, typename T, typename ValueType, class Compare, class Alloc>
inline Serializer<Stream>& operator << (Serializer<Stream>& s, const map<T, ValueType, Compare, Alloc>& value) {
	typedef map<T, ValueType, Compare, Alloc>
											map_type;

	Serializer<Stream> st(TYPE_OBJECT);
	for (typename map_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		try {
			st[lexical_cast<key_type>(it->first)] << it->second;
		}
		catch(const bad_lexical_cast&) {
			return s;
		}
	}

	s.setValue(st);
	return s;
}

/** same key would erase prevous key
template <class Stream, typename T, typename ValueType, class Compare, class Alloc>
inline Serializer<Stream>& operator << (Serializer<Stream>& s, const multimap<T, ValueType, Compare, Alloc>& value) {
	typedef multimap<T, ValueType, Compare, Alloc>
											map_type;

	Serializer<Stream> st(details::TYPE_OBJECT);
	for (typename map_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		key_type key;
		try {
			key = lexical_cast<key_type>(it->first);
		}
		catch(const bad_lexical_cast&) {
			return false;
		}

		st(key) << it->second;
	}

	s.setSerializer(st);
	return s;
}
*/

template <class Stream, typename T, typename ValueType, class Compare, class Alloc>
inline Serializer<Stream>& operator << (Serializer<Stream>& s, const unordered_map<T, ValueType, Compare, Alloc>& value) {
	typedef unordered_map<T, ValueType, Compare, Alloc>
											map_type;

	Serializer<Stream> st(TYPE_OBJECT);
	for (typename map_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		try {
			st[lexical_cast<key_type>(it->first)] << it->second;
		}
		catch(const bad_lexical_cast&) {
			return s;
		}
	}

	s.setValue(st);
	return s;
}

/**
template <class Stream, typename T, typename ValueType, class Compare, class Alloc>
inline Serializer<Stream>& operator << (Serializer<Stream>& s, const unordered_multimap<T, ValueType, Compare, Alloc>& value) {
	typedef unordered_multimap<T, ValueType, Compare, Alloc>
											map_type;

	Serializer<Stream> st(details::TYPE_OBJECT);
	for (typename map_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		key_type key;
		try {
			key = lexical_cast<key_type>(it->first);
		}
		catch(const bad_lexical_cast&) {
			return false;
		}

		st(key) << it->second;
	}

	s.setSerializer(st);
	return s;
}
*/

}}

#endif /* SERIALIZERSTLDETAILS_HPP_ */
