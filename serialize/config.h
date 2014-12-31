/*
 * config.h
 *
 *  Created on: 2014-4-29
 *      Author: Administrator
 */

#ifndef ZERTCORE_SERIALIZE_CONFIG_H_
#define ZERTCORE_SERIALIZE_CONFIG_H_

#include <pch.h>
#include <utils/types.h>

namespace zertcore { namespace serialization {

#ifndef ZT_SERIALIZE_KEY_TYPE
typedef string								key_type;
#else
typedef ZT_SERIALIZE_KEY_TYPE				key_type;
#endif


}}


#endif /* CONFIG_H_ */
