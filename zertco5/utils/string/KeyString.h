/*
 * String.h
 *
 *  Created on: 2015年9月15日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_STRING_KEYSTRING_H_
#define ZERTCORE_UTILS_STRING_KEYSTRING_H_

#include <utils/buffer/SharedBuffer.h>

#include <serialize/Serializer_fwd.h>
#include <serialize/Unserializer_fwd.h>

namespace zertcore { namespace utils {

/**
 * KeyString
 */
class KeyString
{
	friend const u64& getShortData(const KeyString& s) {
		return s.short_data_;
	}
	friend const SharedBuffer& getSharedBuffer(const KeyString& s) {
		return s.data_;
	}

public:
	KeyString();
	KeyString(const KeyString& s);
	KeyString(const char* s);
	KeyString(const char* s, const u32& length);

public:
	u32 size() const;
	bool empty() const;

public:
	/**
	char& operator[] (const u32& index);
	const char& operator[] (const u32& index) const;
	*/

	KeyString& operator= (const KeyString& s);

	bool operator== (const KeyString& s) const;
	bool operator!= (const KeyString& s) const { return !(*this == s); }
	bool operator < (const KeyString& s) const;

public:
	size_t hashCode() const;

private:
	void setData(const char* s, const u32& length);

private:
	union {
	u64							short_data_;
	char						p_[8];
	};

	mutable size_t				hash_code_;
	SharedBuffer				data_;
};

}}

namespace boost {
using namespace zertcore::utils;

template <> struct hash<KeyString>
{
	size_t operator() (const KeyString& s) const {
		return s.hashCode();
	}
};

}

namespace zertcore { namespace serialization {
using namespace zertcore::utils;

template <class Stream>
inline Serializer<Stream>& operator << (Serializer<Stream>& s, const KeyString& v) {
	if (s.getIgnoreNull() && v.empty())
		return s;

	const u64& sd = getShortData(v);
	if (sd) {
		s.setValue(sd);
	}
	else {
		const SharedBuffer& sb = getSharedBuffer(v);
		if (!sb.empty()) {
			s.setValue(sb);
		}
	}

	return s;
}

}}

namespace zertcore { namespace serialization {
using namespace zertcore::utils;

template <class Stream>
inline bool operator >> (const Unserializer<Stream>& s, KeyString& v) {
	return s.getValue(v);
}

}}


#endif /* UTILS_STRING_STRING_H_ */
