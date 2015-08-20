/*
 * DataIO.h
 *
 *  Created on: 2014-10-5
 *      Author: Administrator
 */

#ifndef ZERTCORE_DATAIO_H_
#define ZERTCORE_DATAIO_H_

#include <pch.h>
#include <concurrent/Concurrent.h>
#include <thread/ThreadHandlerSet.h>

#include <object/ActiveObjectTraits.h>

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
		public ObjectBase<DataProvider<Object> >,
		public ThreadHandlerSet<DataProvider<Object> >
{
public:
	typedef Object*							ptr;

public:
	typedef typename ActiveObjectTraits<Object>::ptr
											object_ptr;
	typedef typename ActiveObjectTraits<Object>::id_type
											id_type;

public:
	virtual ~DataProvider() {}

public:
	virtual bool serialize(object_ptr object, const id_type& id)
											= 0;
	virtual bool unserialize(object_ptr object, const id_type& id)
											= 0;
};

}}}

#endif /* DATAIO_H_ */
