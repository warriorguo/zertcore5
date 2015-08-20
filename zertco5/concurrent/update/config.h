/*
 * config.h
 *
 *  Created on: 2015年4月25日
 *      Author: Administrator
 */

#ifndef ZERTCORE_CONCURRENT_TICK_CONFIG_H_
#define ZERTCORE_CONCURRENT_TICK_CONFIG_H_

#include <pch.h>
#include <utils/types.h>

#include <utils/time/Tick.h>

#include "../Concurrent.h"

/**
 * Main goal of tick was concurrent
 */

namespace zertcore { namespace concurrent { namespace update {

typedef ThreadHandler<void (u32)>			updater_type;

/**
 * UpdateConfig
 */
struct UpdateConfig :
		Unserializable<UpdateConfig>, Serializable<UpdateConfig>
{
	tick_type					interval;
	RemoteConfig				rc;

	template <class Archiver>
	void serialize(Archiver& archiver) const {
		archiver["interval"] & interval;
		archiver & rc;
	}

	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		return
			(archiver["interval"] & interval) &&
			(archiver & rc);
	}
};

}}}


#endif /* CONCURRENT_TICK_CONFIG_H_ */
