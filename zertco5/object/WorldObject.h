/*
 * WorldObject.h
 *
 *  Created on: 2014Äê11ÔÂ24ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_WORLDOBJECT_H_
#define ZERTCORE_WORLDOBJECT_H_

#include <pch.h>

#include <core/Runtime.h>
#include <object/PoolObject.h>

namespace zertcore { namespace object {
using namespace zertcore::utils;
}}

namespace zertcore { namespace object {

/**
 * WorldObject<Final>
 */
template <class Final>
class WorldObject :
		public PoolObject<Final>
{
public:
	typedef uuid_t							id_type;

public:
	const static time_type::type			ExpiredTime = 10.0; // default 10 second

public:
	virtual ~WorldObject() {}

public:
	virtual bool init()						= 0;
	virtual void deinit() {}

public:
	virtual void refresh() {}

private:
};

}}


#endif /* WORLDOBJECT_H_ */
