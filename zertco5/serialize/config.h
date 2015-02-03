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

#include <utils/SharedBuffer.h>

namespace zertcore { namespace serialization {

using namespace zertcore::utils;

#ifndef ZC_SERIALIZE_KEY_TYPE
/**
 * Recommand the key length less than 7 (with '\0' its 8 64bits)
 */
typedef string								key_type;
#else
typedef ZC_SERIALIZE_KEY_TYPE				key_type;
#endif


enum value_type {
	TYPE_NONE							= 0,
	TYPE_ARRAY							= 1,
	TYPE_OBJECT							= 2,
};

/**
 * a compromise to query sentence
 */
enum op_code_type
{
	OP_NONE								= 0,
	OP_EQU								= 1,
	OP_NE								= 2,
	OP_GT								= 3,
	OP_GTE								= 4,
	OP_LT								= 5,
	OP_LTE								= 6,
	OP_IN								= 7,
	OP_NIN								= 8,
	OP_AND								= 9,
	OP_OR								= 10,
};

}}


#endif /* CONFIG_H_ */
