/*
 * Impl.hpp
 *
 *  Created on: 2014-4-29
 *      Author: Administrator
 */
#ifndef ZERTCORE_SERIALIZE_IMPL_H_
#define ZERTCORE_SERIALIZE_IMPL_H_

#include <pch.h>

#include <utils/types.h>

#include <serialize/Archive.h>
#include <serialize/Serialize.h>

namespace zertcore {namespace serialization {
/**
template <typename Final, typename ValueType, class Alloc>
inline Final& IArchiver<Final>::operator << (const list<ValueType, Alloc>& value) {
	typedef list<ValueType, Alloc>			list_type;

	Final& ar = list();
	for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		ar << *it;
	}

	return *static_cast<Final *>(this);
}

template <typename Final, typename ValueType, class Alloc>
inline Final& IArchiver<Final>::operator << (const vector<ValueType, Alloc>& value) {
	typedef vector<ValueType, Alloc>		list_type;

	Final& ar = list();
	for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		ar << *it;
	}

	return *static_cast<Final *>(this);
}

template <typename Archiver, typename ValueType, class Alloc>
inline Archiver& operator << (Archiver& archiver, const deque<ValueType, Alloc>& value) {
	typedef deque<ValueType, Alloc>			list_type;

	Archiver& ar = archiver.template list();
	for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		ar << *it;
	}

	return archiver;
}

template <typename Archiver, typename ValueType, class Compare, class Alloc>
inline Archiver& operator << (Archiver& archiver, const set<ValueType, Compare, Alloc>& value) {
	typedef set<ValueType, Compare, Alloc>	list_type;

	Archiver& ar = archiver.template list();
	for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		ar << *it;
	}

	return archiver;
}

template <typename Archiver, typename ValueType, class Compare, class Alloc>
inline Archiver& operator << (Archiver& archiver, const multiset<ValueType, Compare, Alloc>& value) {
	typedef multiset<ValueType, Compare, Alloc>
											list_type;

	Archiver& ar = archiver.template list();
	for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		ar << *it;
	}

	return archiver;
}

template <typename Archiver, typename T, typename ValueType, class Compare, class Alloc>
inline Archiver& operator << (Archiver& archiver, const map<T, ValueType, Compare, Alloc>& value) {
	typedef map<T, ValueType, Compare, Alloc>
											map_type;

	Archiver& ar = archiver.template object();
	for (typename map_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		key_type key;
		try {
			key = lexical_cast<key_type>(it->first);
		}
		catch(const bad_lexical_cast&) {
			return false;
		}

		ar(key) << it->second;
	}

	return archiver;
}

template <typename Archiver, typename T, typename ValueType, class Compare, class Alloc>
inline Archiver& operator << (Archiver& archiver, const multimap<T, ValueType, Compare, Alloc>& value) {
	typedef multimap<T, ValueType, Compare, Alloc>
											map_type;

	Archiver& ar = archiver.template object();
	for (typename map_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		key_type key;
		try {
			key = lexical_cast<key_type>(it->first);
		}
		catch(const bad_lexical_cast&) {
			return false;
		}

		ar(key) << it->second;
	}

	return archiver;
}
*/
} /* namespace serialization */} /* namespace zertcore */

#endif
