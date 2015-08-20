/*
 * UUIDGenerator.h
 *
 *  Created on: 2015年5月4日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_UUID_UUIDGENERATOR_H_
#define ZERTCORE_UTILS_UUID_UUIDGENERATOR_H_

#include <pch.h>
#include <utils/types.h>

namespace zertcore { namespace utils {

/**
 * UUIDGenerator
 */
class UUIDGenerator
{
public:
	static uuid_t generate();

private:
};

}}


#endif /* UTILS_UUID_UUIDGENERATOR_H_ */
