/*
 * String.cpp
 *
 *  Created on: 2015年9月15日
 *      Author: Administrator
 */

#include "KeyString.h"
#include <log/Log.h>

namespace zertcore { namespace utils {

KeyString::KeyString() : short_data_(0), hash_code_(0) {
	;
}

KeyString::KeyString(const KeyString& s) {
	*this = s;
}

KeyString::KeyString(const char* s) {
	setData(s, (u32)strlen(s));
}

KeyString::KeyString(const char* s, const u32& length) {
	setData(s, length);
}

size_t KeyString::
hashCode() const {
	if (short_data_) return short_data_;
	if (hash_code_) return hash_code_;

	hash<const char *> hasher;
	return hash_code_ = hasher((const char *)data_.data());
}

u32 KeyString::
size() const {
	if (short_data_) {
		u32 l = 0;
		for (;l < 8 && p_[l]; ++l);
		return l;
	}

	return data_.size();
}

bool KeyString::
empty() const {
	if (short_data_) {
		return false;
	}

	if (!data_.empty()) {
		return false;
	}

	return true;
}

/**
char& KeyString::
operator[] (const u32& index) {
	if (index >= 8) {
		if (data_.size() <= index) {
			ZCLOG(FATAL) << "out of range index=" << index
					<< " data size=" << data_.size() << End;
		}

		return data_[index];
	}

	hash_code_ = 0;
	return p_[index];
}


const char& KeyString::
operator[] (const u32& index) const {
	if (index >= 8) {
		if (data_.size() <= index) {
			ZCLOG(FATAL) << "out of range index=" << index
					<< " data size=" << data_.size() << End;
		}

		return data_[index];
	}

	return p_[index];
}
*/

bool KeyString::
operator < (const KeyString& s) const {
	return hashCode() < s.hashCode();
}

KeyString& KeyString::
operator= (const KeyString& s) {
	if (s.short_data_) {
		short_data_ = s.short_data_;
	}
	else {
		data_ = s.data_;
		hash_code_ = s.hash_code_;
	}

	return *this;
}

bool KeyString::
operator== (const KeyString& s) const {
	if (short_data_) {
		return short_data_ == s.short_data_;
	}

	return data_ == s.data_;
}

void KeyString::
setData(const char* s, const u32& length) {
	short_data_ = 0;
	hash_code_ = 0;

	if (length >= 8) {
		data_.assign(s, length);
		return ;
	}
	for (u32 i = 0; i < length; ++i) {
		p_[i] = s[i];
	}
}

}}

