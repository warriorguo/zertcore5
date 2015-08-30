/*
 * WorldObjectManager.h
 *
 *  Created on: 2014��11��24��
 *      Author: Administrator
 */

#ifndef WORLDOBJECTMANAGER_H_
#define WORLDOBJECTMANAGER_H_

#include <pch.h>

#include <core/Runtime.h>

#include <db/io/DataProviderManager.h>
#include <utils/Singleton.h>
#include <thread/ThreadHandler.h>
#include <thread/ThreadHandlerSet.h>
#include <utils/ExpiredObjectMap.h>
#include <concurrent/rpc/config.h>

#include "ObjectTraits.h"
#include "ObjectBase.h"
#include "PoolObject.h"
#include "ActiveObject.h"

namespace zertcore { namespace object {
using namespace zertcore::utils;
using namespace zertcore::db;
using namespace zertcore::concurrent;
}}

namespace zertcore { namespace object {

/**
 * ActiveObjectManager<Final, Object>
 */
template <class Final, class Object>
class ActiveObjectManager :
		public Singleton<Final>,
		public ActiveObjectManagerBase<Object>
{
public:
	typedef Final							final_type;
	typedef Object							object_type;

public:
	typedef ThreadHandler<void ()>			handler_type;

public:
	typedef typename ActiveObjectTraits<object_type>::ptr
											object_ptr;
	typedef typename ActiveObjectTraits<object_type>::id_type
											object_id_type;

	typedef io::DataProviderManager<object_type>
											dp_manager;
	typedef typename dp_manager::data_provider_ptr
											dp_ptr;
public:
	struct Cell
	{
		object_id_type			id;
		object_ptr				object;
		dp_ptr					provider;
	};

	struct CellGenerate :
			public Cell,
			public PoolObject<CellGenerate>
	{
		ConcurrentState::ptr	state;

		ZC_TO_STRING("id" << Cell::id);
	};

	typedef ExpiredObjectMap<object_id_type, Cell>
											cell_expired_map_type;

public:
	virtual ~ActiveObjectManager() {}

public:
	/**
	 * should be called in game init,
	 * called in instance first called
	 */
	virtual void init();

public:
	/**
	 * implement from ManagerBase
	 */
	virtual void saveObject(const object_id_type& id);
	virtual bool reloadObject(const object_id_type& id, object_ptr object);

	virtual bool setupSlave(const object_id_type& id, object_ptr object);

public:
	bool setupSync(const rpc::condition_expr_type& expr = rpc::condition_expr_type());

private:
	void onSlaveSync(rpc::key_type key, rpc::oarchiver_type params);

public:
	virtual object_ptr get(object_id_type id, io::key_type provider_type = 0);
	virtual object_ptr create(object_id_type id, io::key_type provider_type);

public:
	virtual void onRelease(object_id_type id) {}

public:
	bool reload(object_id_type id);
	/**
	 * the prepare would return directly
	 * the handler was callback when the fetch
	 */
	virtual bool prepare(object_id_type id, io::key_type provider_type, ConcurrentState::ptr state = ConcurrentState::ptr());
	virtual void save(object_id_type id, ConcurrentState::ptr state = ConcurrentState::ptr());
	void save(object_ptr object, ConcurrentState::ptr state = ConcurrentState::ptr());

public:
	/**
	 * these two modules would block until provider fetch the data,
	 * and invoked initObject()
	 */
//	virtual object_ptr fetch(const uuid_t& id, const io::key_type& provider_type);
/**
public:
	virtual bool initObject(object_ptr object);
*/
protected:
	virtual void setupObject(typename CellGenerate::ptr cell, RuntimeContext::ptr ctx);

protected:
	cell_expired_map_type		cell_expired_map_;
};

}}

#endif /* WORLDOBJECTMANAGER_H_ */
