/*
 * KeysHolder.h
 *
 *  Created on: 2015年10月9日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_CONDITION_KEYSHOLDER_H_
#define ZERTCORE_UTILS_CONDITION_KEYSHOLDER_H_

#include <pch.h>
#include <utils/types.h>

namespace zertcore { namespace utils { namespace details {

/**
 * KeysHolder<KeyType>
 */
template <typename KeyType>
class KeysHolder
{
public:
	KeysHolder() {}
	KeysHolder(const KeyType& v) {
		key_list_.push_back(v);
	}

public:
	KeysHolder& operator() (const KeyType& v) {
		key_list_.push_back(v);
		return *this;
	}

public:
	vector<KeyType>& keyList() { return key_list_; }
	const vector<KeyType>& keyList() const { return key_list_; }

private:
	vector<KeyType>				key_list_;
};

}}}

#include <serialize/Serializer_fwd.h>
#include <serialize/Unserializer_fwd.h>

namespace zertcore { namespace serialization {
using namespace zertcore::utils::details;

template <class Stream, typename KeyType>
inline Serializer<Stream>& operator << (Serializer<Stream>& s, const KeysHolder<KeyType>& v) {
	s << v.keyList();
	return s;
}

template <class Stream, typename KeyType>
inline bool operator >> (const Unserializer<Stream>& s, KeysHolder<KeyType>& v) {
	return s >> v.keyList();
}

}}

#endif /* UTILS_CONDITION_KEYSHOLDER_H_ */
