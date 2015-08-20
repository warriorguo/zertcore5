/*
 * TickTypeSerialization.hpp
 *
 *  Created on: 2015年5月8日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_TIME_DETAILS_TICKTYPESERIALIZATION_HPP_
#define ZERTCORE_UTILS_TIME_DETAILS_TICKTYPESERIALIZATION_HPP_

#include <serialize/Serializer_fwd.h>
#include <serialize/Unserializer_fwd.h>

namespace zertcore { namespace serialization {
using namespace zertcore::time_utils;

template <class Stream>
inline Serializer<Stream>& operator << (Serializer<Stream>& s, const Tick& v) {
	s.setValue(v.value());
	return s;
}

template <class Stream>
inline bool operator >> (const Unserializer<Stream>& s, Tick& v) {
	return s.getValue(v.value());
}

}}



#endif /* UTILS_TIME_DETAILS_TICKTYPESERIALIZATION_HPP_ */
