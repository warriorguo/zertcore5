/*
 * UnserializeImpl.hpp
 *
 *  Created on: 2014Äê4ÔÂ29ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_UNSERIALIZE_IMPL_H_
#define ZERTCORE_UNSERIALIZE_IMPL_H_

#include <pch.h>

#include <utils/types.h>

#include <serialize/Serialize.h>

namespace zertcore {namespace serialization {
using namespace boost;
}}

namespace zertcore {namespace serialization {

template <typename Archiver, typename ValueType, class Alloc>
inline bool operator >> (Archiver& archiver, list<ValueType, Alloc>& value) {
//	typedef list<ValueType, Alloc>			list_type;

	value.clear();

	Archiver ar;
	if (!archiver.template begin(ar)) {
		return false;
	}

	for (; ar.template isValid(); ar.template next()) {
		ValueType v;
		if (ar >> v)
			value.push_back(v);
	}

	return !value.empty();
}

template <typename Archiver, typename ValueType, class Alloc>
inline bool operator >> (Archiver& archiver, vector<ValueType, Alloc>& value) {
//	typedef vector<ValueType, Alloc>		list_type;

	value.clear();

	Archiver ar;
	if (!archiver.template begin(ar)) {
		return false;
	}

	for (; ar.template isValid(); ar.template next()) {
		ValueType v;
		if (ar >> v)
			value.push_back(v);
	}

	return !value.empty();
}

template <typename Archiver, typename ValueType, class Alloc>
inline bool operator >> (Archiver& archiver, deque<ValueType, Alloc>& value) {
//	typedef deque<ValueType, Alloc>			list_type;

	value.clear();

	Archiver ar;
	if (!archiver.template begin(ar)) {
		return false;
	}

	for (; ar.template isValid(); ar.template next()) {
		ValueType v;
		if (ar >> v)
			value.push_back(v);
	}

	return !value.empty();
}

template <typename Archiver, typename ValueType, class Compare, class Alloc>
inline bool operator >> (Archiver& archiver, set<ValueType, Compare, Alloc>& value) {
//	typedef set<ValueType, Compare, Alloc>	list_type;

	value.clear();

	Archiver ar;
	if (!archiver.template begin(ar)) {
		return false;
	}

	for (; ar.template isValid(); ar.template next()) {
		ValueType v;
		if (ar >> v)
			value.insert(v);
	}

	return !value.empty();
}

template <typename Archiver, typename ValueType, class Compare, class Alloc>
inline bool operator >> (Archiver& archiver, multiset<ValueType, Compare, Alloc>& value) {
	/**
	typedef multiset<ValueType, Compare, Alloc>
											list_type;
	*/
	value.clear();

	Archiver ar;
	if (!archiver.template begin(ar)) {
		return false;
	}

	for (; ar.template isValid(); ar.template next()) {
		ValueType v;
		if (ar >> v)
			value.insert(v);
	}

	return !value.empty();
}

template <typename Archiver, typename T, typename ValueType, class Compare, class Alloc>
inline bool operator >> (Archiver& archiver, map<T, ValueType, Compare, Alloc>& value) {
	/**
	typedef map<T, ValueType, Compare, Alloc>
											map_type;
	*/
	value.clear();

	Archiver ar;
	if (!archiver.template begin(ar)) {
		return false;
	}

	for (; ar.template isValid(); ar.template next()) {
		ValueType v;
		if (ar >> v) {
			T key;
			try {
				key = lexical_cast<T>(ar.template key());
			}
			catch(bad_lexical_cast&) {
				continue;
			}

			value[key] = v;
		}
	}

	return !value.empty();
}

template <typename Archiver, typename T, typename ValueType, class Compare, class Alloc>
inline bool operator >> (Archiver& archiver, multimap<T, ValueType, Compare, Alloc>& value) {
	/**
	typedef multimap<T, ValueType, Compare, Alloc>
											map_type;
	*/
	value.clear();

	Archiver ar;
	if (!archiver.template begin(ar)) {
		return false;
	}

	for (; ar.template isValid(); ar.template next()) {
		ValueType v;
		if (ar >> v) {
			T key;
			try {
				key = lexical_cast<T>(ar.template key());
			}
			catch(bad_lexical_cast&) {
				continue;
			}

			value[key] = v;
		}
	}

	return !value.empty();
}

} /* namespace serialization */} /* namespace zertcore */

#endif /* UNSERIALIZEIMPL_HPP_ */
