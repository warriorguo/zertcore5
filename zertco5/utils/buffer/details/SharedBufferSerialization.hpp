/*
 * SharedBufferSerialization.hpp
 *
 *  Created on: 2015年5月25日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_BUFFER_DETAILS_SHAREDBUFFERSERIALIZATION_HPP_
#define ZERTCORE_UTILS_BUFFER_DETAILS_SHAREDBUFFERSERIALIZATION_HPP_

#include <serialize/Serializer_fwd.h>
#include <serialize/Unserializer_fwd.h>

namespace zertcore { namespace serialization {
using namespace zertcore::utils;


/**
 * NOTICE: these two serialization function JUST support BSON since just bson support bin data!
 *
 */
template <class Stream>
inline Serializer<Stream>& operator << (Serializer<Stream>& s, const SharedBuffer& v) {
	s.setValue(v);
	return s;
}

template <class Stream>
inline bool operator >> (const Unserializer<Stream>& s, SharedBuffer& v) {
	return s.getValue(v);
}

}}


#endif /* UTILS_BUFFER_DETAILS_SHAREDBUFFERSERIALIZATION_HPP_ */
