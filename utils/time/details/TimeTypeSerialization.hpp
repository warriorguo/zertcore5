/*
 * TimeTypeSerialization.h
 *
 *  Created on: 2014-10-5
 *      Author: Administrator
 */

#ifndef ZERTCORE_TIMETYPESERIALIZATION_H_
#define ZERTCORE_TIMETYPESERIALIZATION_H_

#include <serialize/Serialize.h>
#include <serialize/Unserialize.h>

namespace zertcore { namespace serialization {
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
