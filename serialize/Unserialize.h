/*
 * Unserialize.h
 *
 *  Created on: 2014Äê4ÔÂ29ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_UNSERIALIZE_H_
#define ZERTCORE_UNSERIALIZE_H_

#include <pch.h>

#include <serialize/config.h>

namespace zertcore { namespace serialization {

/**
template <class Archiver, typename T>
bool operator >> (Archiver& archiver, T& v);

template <typename Archiver>
bool operator >> (Archiver& archiver, char& value);

template <typename Archiver>
bool operator >> (Archiver& archiver, unsigned char& value);

template <typename Archiver>
bool operator >> (Archiver& archiver, short& value);

template <typename Archiver>
bool operator >> (Archiver& archiver, unsigned short& value);

template <typename Archiver>
bool operator >> (Archiver& archiver, int& value);

template <typename Archiver>
bool operator >> (Archiver& archiver, unsigned int& value);

template <typename Archiver>
bool operator >> (Archiver& archiver, long& value);

template <typename Archiver>
bool operator >> (Archiver& archiver, unsigned long& value);

template <typename Archiver>
bool operator >> (Archiver& archiver, long long& value);

template <typename Archiver>
bool operator >> (Archiver& archiver, unsigned long long& value);

template <typename Archiver>
bool operator >> (Archiver& archiver, float& value);

template <typename Archiver>
bool operator >> (Archiver& archiver, double& value);

template <typename Archiver>
bool operator >> (Archiver& archiver, std::string& value);

template <typename Archiver, typename ValueType, class Alloc>
bool operator >> (Archiver& archiver, std::list<ValueType, Alloc>& value);

template <typename Archiver, typename ValueType, class Alloc>
bool operator >> (Archiver& archiver, std::vector<ValueType, Alloc>& value);

template <typename Archiver, typename ValueType, class Alloc>
bool operator >> (Archiver& archiver, std::deque<ValueType, Alloc>& value);

template <typename Archiver, typename ValueType, class Compare, class Alloc>
bool operator >> (Archiver& archiver, std::set<ValueType, Compare, Alloc>& value);

template <typename Archiver, typename ValueType, class Compare, class Alloc>
bool operator >> (Archiver& archiver, std::multiset<ValueType, Compare, Alloc>& value);

template <typename Archiver, typename T, typename ValueType, class Compare, class Alloc>
bool operator >> (Archiver& archiver, std::map<T, ValueType, Compare, Alloc>& value);

template <typename Archiver, typename T, typename ValueType, class Compare, class Alloc>
bool operator >> (Archiver& archiver, std::multimap<T, ValueType, Compare, Alloc>& value);
*/

template <class F>
struct Unserializable
{
/**
	template <class Archiver>
	friend inline bool operator >> (Archiver& archiver, F& value) {
		return value.template unserialize(archiver);
	}
*/
};

}}

/**
#include <serialize/details/UnserializeImpl.hpp>
*/

#endif /* UNSERIALIZE_H_ */
