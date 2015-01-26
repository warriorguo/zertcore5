/*
 * WorldObjectManager.h
 *
 *  Created on: 2014Äê11ÔÂ24ÈÕ
 *      Author: Administrator
 */

#ifndef WORLDOBJECTMANAGER_H_
#define WORLDOBJECTMANAGER_H_

#include <pch.h>

#include <core/Runtime.h>

#include <db/io/DataProviderManager.h>

#include <object/ObjectTraits.h>
#include <object/ObjectBase.h>

#include <utils/Singleton.h>

namespace zertcore { namespace object {
using namespace zertcore::utils;
using namespace zertcore::db;
}}

namespace zertcore { namespace object {

/**
 * ObjectManagerBase<Final, Object>
 */
template <class Final, class WorldObject>
class WorldObjectManager :
		public Singleton<WorldObjectManager<Final, WorldObject> >
{
public:
	typedef Final							final_type;
	typedef WorldObject						object_type;

public:
	typedef typename ObjectTraits<object_type>::ptr
											object_ptr;
	typedef io::DataProviderManager<object_type>
											dp_manager;
	typedef typename io::DataProvider<object_type>::ptr
											dp_ptr;

public:
	struct Cell
	{
		uuid_t					id;
		object_ptr				ptr;
		dp_ptr					provider;

		Cell() : id(0), provider(null) {}
		Cell(const uuid_t& id0, const io::key_type& type) : id(id0) {
			provider = dp_manager::Instance().get(provider_type);
		}
	};
	typedef map<uuid_t, Cell>				cell_map_type;
	typedef multimap<time_type::type, cell_map_type::iterator>
											cell_expired_map_type;

public:
	virtual ~WorldObjectManager() {}

public:
	/**
	 * should be called in game init
	 */
	virtual bool init()						= 0;
	virtual bool deinit() {
		return Singleton<WorldObjectManager<final_type, object_type> >::deinit();
	}

public:
	virtual object_ptr get(const uuid_t& id);
	virtual object_ptr create(const io::key_type& provider_type);

public:
	/**
	 * the prepare would return directly
	 * the handler was callback when the fetch
	 */
	virtual bool prepare(const uuid_t& id,
			const io::key_type& provider_type, ConcurrentState::ptr state);

public:
	/**
	 * these two modules would block until provider fetch the data,
	 * and invoked initObject()
	 */
	virtual object_ptr fetch(const uuid_t& id, const io::key_type& provider_type);
	virtual bool save(object_ptr object);

public:
	virtual bool initObject(object_ptr object);

protected:
	virtual void setupObject(Cell cell);

protected:
	cell_map_type				cell_map_;
	cell_expired_map_type		cell_expired_map_;
};

}}

#include <object/details/WorldObjectManagerImpl.hpp>

#endif /* WORLDOBJECTMANAGER_H_ */
