/*
 * WorldObjectTraits.h
 *
 *  Created on: 2015年6月6日
 *      Author: Administrator
 */

#ifndef ZERTCORE_OBJECT_WORLDOBJECTTRAITS_H_
#define ZERTCORE_OBJECT_WORLDOBJECTTRAITS_H_

#include <utils/types.h>
#include "ObjectTraits.h"

#include <utils/time/Tick.h>

namespace zertcore {

/**
 * play role
 */
enum {
	ROLE_NONE								= 0,
	ROLE_SLAVE								= 1 << 0,
	ROLE_MASTER								= 1 << 1,
};

typedef u32									role_type;

}

namespace zertcore { namespace object {

template <class Object>
struct ActiveObjectTraits : public ObjectTraits<Object>
{
	typedef uuid_t							id_type;

	static const char*			TABLE_NAME;
	static const char*			RPC_NAME;
	static const char*			SYNC_NAME;

	static const tick_type		DefaultExpiredTick; // default 10 second
};

template <class Object>
const char*						ActiveObjectTraits<Object>::TABLE_NAME = NULL;

template <class Object>
const char*						ActiveObjectTraits<Object>::RPC_NAME = NULL;

template <class Object>
const char*						ActiveObjectTraits<Object>::SYNC_NAME = NULL;

template <class Object>
const tick_type					ActiveObjectTraits<Object>::DefaultExpiredTick = 10000;

}}


#endif /* OBJECT_WORLDOBJECTTRAITS_H_ */
