/*
 * UnserializerSTLDetails.hpp
 *
 *  Created on: 2015Äê1ÔÂ19ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_UNSERIALIZERSTLDETAILS_HPP_
#define ZERTCORE_UNSERIALIZERSTLDETAILS_HPP_

namespace zertcore { namespace serialization {

template <class Stream, typename ValueType, class Alloc>
inline bool operator >> (Unserializer<Stream>& s, list<ValueType, Alloc>& value) {

	Unserializer<Stream> st(details::TYPE_ARRAY);
	if (!st.getSerializer(s))
		return false;

	while(true) {
		ValueType v;
		if (!(st >> v)) {
			break;
		}

		value.push_back(v);
	}

	return !value.empty();
}

template <class Stream, typename ValueType, class Alloc>
inline bool operator >> (Unserializer<Stream>& s, vector<ValueType, Alloc>& value) {

	Unserializer<Stream> st(details::TYPE_ARRAY);
	if (!st.getSerializer(s))
		return false;

	while(true) {
		ValueType v;
		if (!(st >> v)) {
			break;
		}

		value.push_back(v);
	}

	return !value.empty();
}

template <class Stream, typename ValueType, class Alloc>
inline bool operator >> (Unserializer<Stream>& s, deque<ValueType, Alloc>& value) {

	Unserializer<Stream> st(details::TYPE_ARRAY);
	if (!st.getSerializer(s))
		return false;

	while(true) {
		ValueType v;
		if (!(st >> v)) {
			break;
		}

		value.push_back(v);
	}

	return !value.empty();
}

template <class Stream, typename ValueType, class Compare, class Alloc>
inline bool operator >> (Unserializer<Stream>& s, set<ValueType, Compare, Alloc>& value) {

	Unserializer<Stream> st(details::TYPE_ARRAY);
	if (!st.getSerializer(s))
		return false;

	while(true) {
		ValueType v;
		if (!(st >> v)) {
			break;
		}

		value.insert(v);
	}

	return !value.empty();
}

template <class Stream, typename ValueType, class Compare, class Alloc>
inline bool operator >> (Unserializer<Stream>& s, multiset<ValueType, Compare, Alloc>& value) {

	Unserializer<Stream> st(details::TYPE_ARRAY);
	if (!st.getSerializer(s))
		return false;

	while(true) {
		ValueType v;
		if (!(st >> v)) {
			break;
		}

		value.insert(v);
	}

	return !value.empty();
}

template <class Stream, typename ValueType, class Compare, class Alloc>
inline bool operator >> (Unserializer<Stream>& s, unordered_set<ValueType, Compare, Alloc>& value) {

	Unserializer<Stream> st(details::TYPE_ARRAY);
	if (!st.getSerializer(s))
		return false;

	while(true) {
		ValueType v;
		if (!(st >> v)) {
			break;
		}

		value.insert(v);
	}

	return !value.empty();
}

template <class Stream, typename ValueType, class Compare, class Alloc>
inline bool operator >> (Unserializer<Stream>& s, unordered_multiset<ValueType, Compare, Alloc>& value) {

	Unserializer<Stream> st(details::TYPE_ARRAY);
	if (!st.getSerializer(s))
		return false;

	while(true) {
		ValueType v;
		if (!(st >> v)) {
			break;
		}

		value.insert(v);
	}

	return !value.empty();
}

template <class Stream, typename T, typename ValueType, class Compare, class Alloc>
inline bool operator >> (Unserializer<Stream>& s, map<T, ValueType, Compare, Alloc>& value) {
	typedef map<T, ValueType, Compare, Alloc>
											map_type;

	Unserializer<Stream> st(details::TYPE_ARRAY);
	if (!st.getSerializer(s))
		return false;

	while(true) {
		ValueType v;
		if (!(st >> v)) {
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
inline bool operator >> (Unserializer<Stream>& s, unordered_map<T, ValueType, Compare, Alloc>& value) {
	typedef unordered_map<T, ValueType, Compare, Alloc>
											map_type;

	Unserializer<Stream> st(details::TYPE_ARRAY);
	if (!st.getSerializer(s))
		return false;

	while(true) {
		ValueType v;
		if (!(st >> v)) {
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
