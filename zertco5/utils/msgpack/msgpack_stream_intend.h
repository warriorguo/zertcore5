/*
 * msgpack_stream_intend.h
 *
 *  Created on: 2015Äê1ÔÂ23ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_MSGPACK_STREAM_INTEND_H_
#define ZERTCORE_MSGPACK_STREAM_INTEND_H_

#include <map>
#include <string>
#include <iostream>

#include <msgpack_fwd.hpp>
#include <boost/smart_ptr.hpp>

namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
struct zc_safe_input_object;
void operator<< (object& o, const zc_safe_input_object& v);
}}

#include <msgpack.hpp>

namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {

/**
 * IMPORTANT: just for the input for the serialization!
 * DONT use this for unserialization, object was good enough :)
 */
struct zc_safe_input_object : public object
{
	typedef boost::shared_ptr<char[]>		smart_ptr;
	smart_ptr					ptr;

	zc_safe_input_object() {}

	template <typename T>
	explicit zc_safe_input_object(const T& v);

	template <typename T>
	zc_safe_input_object& operator=(const T& v);

	void realloc(bool dump = false);

	void dump() {
		realloc(true);
	}
};
}}

#include <serialize/config.h>

namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
typedef std::map<::zertcore::serialization::key_type, zc_safe_input_object>
											input_object_map_type;
typedef std::vector<zc_safe_input_object>	input_object_array_type;
typedef std::map<::zertcore::serialization::key_type, object>
											output_object_map_type;
typedef std::vector<object>					output_object_array_type;

inline void operator<< (zc_safe_input_object& o, const input_object_map_type& v) {
	o.type = msgpack::type::MAP;
	o.via.map.size = v.size();
	o.realloc();

	size_t index = 0;
	for (auto it = v.begin(); it != v.end(); ++it, ++index) {
		o.via.map.ptr[index].key = it->first;
		o.via.map.ptr[index].val = it->second;
	}
}

inline void operator<< (zc_safe_input_object& o, const input_object_array_type& v) {
	o.type = msgpack::type::ARRAY;
	o.via.map.size = v.size();
	o.realloc();

	size_t index = 0;
	for (auto it = v.begin(); it != v.end(); ++it, ++index) {
		o.via.array.ptr[index] = *it;
	}
}

inline void operator<< (object& o, const zc_safe_input_object& v) {
	o = (const object&)v;
}

inline void operator<< (zc_safe_input_object& o, const bool& v) {
	operator << ((object &)o, v);
}
inline void operator<< (zc_safe_input_object& o, const int8_t& v) {
	operator << ((object &)o, v);
}
inline void operator<< (zc_safe_input_object& o, const uint8_t& v) {
	operator << ((object &)o, v);
}
inline void operator<< (zc_safe_input_object& o, const int16_t& v) {
	operator << ((object &)o, v);
}
inline void operator<< (zc_safe_input_object& o, const uint16_t& v) {
	operator << ((object &)o, v);
}
inline void operator<< (zc_safe_input_object& o, const int32_t& v) {
	operator << ((object &)o, v);
}
inline void operator<< (zc_safe_input_object& o, const uint32_t& v) {
	operator << ((object &)o, v);
}
inline void operator<< (zc_safe_input_object& o, const int64_t& v) {
	operator << ((object &)o, v);
}
inline void operator<< (zc_safe_input_object& o, const uint64_t& v) {
	operator << ((object &)o, v);
}
inline void operator<< (zc_safe_input_object& o, const float& v) {
	operator << ((object &)o, v);
}
inline void operator<< (zc_safe_input_object& o, const double& v) {
	operator << ((object &)o, v);
}
inline void operator<< (zc_safe_input_object& o, const char* v) {
	operator << ((object &)o, v);
	o.dump();
}
inline void operator<< (zc_safe_input_object& o, const std::string& v) {
	operator << ((object &)o, v);
	o.dump();
}

template <typename T>
zc_safe_input_object::zc_safe_input_object(const T& v) {
	operator << (*this, v);
}
template <typename T>
zc_safe_input_object& zc_safe_input_object::operator=(const T& v) {
	operator << (*this, v);
	return *this;
}

}}



#endif /* MSGPACK_STREAM_INTEND_H_ */
