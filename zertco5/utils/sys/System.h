/*
 * System.h
 *
 *  Created on: 2015年6月24日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_SYS_SYSTEM_H_
#define ZERTCORE_UTILS_SYS_SYSTEM_H_

#include <pch.h>
#include <utils/types.h>

namespace zertcore { namespace utils {

/**
 * System
 */
class System
{
public:
	static u32 CPUCoreCount() {
		return sysconf(_SC_NPROCESSORS_ONLN);
	}
};

}}


#endif /* UTILS_SYS_SYSTEM_H_ */
