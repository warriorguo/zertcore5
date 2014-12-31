/*
 * DataIO.h
 *
 *  Created on: 2014-10-5
 *      Author: Administrator
 */

#ifndef ZERTCORE_DATAIO_H_
#define ZERTCORE_DATAIO_H_

#include <pch.h>
#include <object/ObjectBase.h>
#include <concurrent/Concurrent.h>

namespace zertcore { namespace db { namespace io {
using namespace zertcore::object;
using namespace zertcore::concurrent;
}}}

namespace zertcore { namespace db { namespace io {

/**
 * DataProvider<Object>
 */
template <class Object>
class DataProvider :
		public ObjectBase<DataProvider<Object> >
{
public:
	typedef typename Object::ptr			object_ptr;
	typedef typename Object::id_type		id_type;

public:
	virtual ~DataProvider() {}

public:
	virtual bool serialize(object_ptr object, const id_type& id)
											= 0;
	virtual bool unserialize(object_ptr object, const id_type& id)
											= 0;

public:
	virtual ThreadIndex getThreadIndex(const id_type& id)
											= 0;
};

}}}

#endif /* DATAIO_H_ */
