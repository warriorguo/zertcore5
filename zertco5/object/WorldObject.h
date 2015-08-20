/*
 * SerializeObject.h
 *
 *  Created on: 2015年6月16日
 *      Author: Administrator
 */

#ifndef ZERTCORE_OBJECT_WORLDOBJECT_H_
#define ZERTCORE_OBJECT_WORLDOBJECT_H_

#include <pch.h>
#include <utils/types.h>

#include "utils/serializable/SerializableObject.h"

namespace zertcore { namespace object {
using namespace zertcore::utils;
}}

namespace zertcore { namespace object {

/**
 * WorldObject
 */
template <class Final>
struct WorldObject :
		public ActiveObject<Final>,
		public SerializableObject
{
};


}}

namespace zertcore { namespace object {

/**
 * WorldObjectManager
 */
template <class Final, class Object>
class WorldObjectManager :
		public ActiveObjectManager<Final, Object>
{
public:
	virtual ~WorldObjectManager() {}

public:
	virtual void init() {
		ActiveObjectManager<Final, Object>::init();
		ActiveObjectManager<Final, Object>::dp_manager::Instance().
				reg(PROVIDER_MONGODB, new io::MongoDBDataProvider<Object>);
	}
};

}}

#endif /* OBJECT_WORLDOBJECT_H_ */
