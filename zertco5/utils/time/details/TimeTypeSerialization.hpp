/*
 * TimeTypeSerialization.h
 *
 *  Created on: 2014-10-5
 *      Author: Administrator
 */

#ifndef ZERTCORE_TIMETYPESERIALIZATION_H_
#define ZERTCORE_TIMETYPESERIALIZATION_H_

#include <serialize/Serializer_fwd.h>
#include <serialize/Unserializer_fwd.h>

namespace zertcore { namespace serialization {
using namespace zertcore::time_utils;

template <class Stream>
inline Serializer<Stream>& operator << (Serializer<Stream>& s, const TimeType& v) {
	s.setValue(v.value);
	return s;
}

template <class Stream>
inline bool operator >> (const Unserializer<Stream>& s, TimeType& v) {
	return s.getValue(v.value);
}

/**
template <typename Archiver>
inline Archiver& operator << (Archiver& archiver, const time_type& time) {
	archiver << time.value;
	return true;
}
template <typename Archiver>
inline bool operator >> (Archiver& archiver, time_type& time) {
	archiver >> time.value;
	return true;
}
*/

}}

#endif /* TIMETYPESERIALIZATION_H_ */
