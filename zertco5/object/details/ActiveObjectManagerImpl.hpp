/*
 * WorldObjectManagerImpl.hpp
 *
 *  Created on: 2014��11��25��
 *      Author: Administrator
 */

#ifndef ZERTCORE_WORLDOBJECTMANAGERIMPL_HPP_
#define ZERTCORE_WORLDOBJECTMANAGERIMPL_HPP_

#include <pch.h>

#include <concurrent/Concurrent.h>
#include <concurrent/rpc/RPC.h>
#include <thread/ThreadHandler.h>
#include "../ActiveObjectManager.h"

namespace zertcore { namespace object {
using namespace zertcore::concurrent;
}}

namespace zertcore { namespace object {

template <class Final, class Object>
void ActiveObjectManager<Final, Object>::
init() {
	Singleton<Final>::init();

	cell_expired_map_.init(bind(&ActiveObjectManager::onRelease, this, _1),
				ActiveObjectTraits<object_type>::DefaultExpiredTick);
}

template <class Final, class Object>
void ActiveObjectManager<Final, Object>::
saveObject(const object_id_type& id) {
	save(id);
}

template <class Final, class Object>
bool ActiveObjectManager<Final, Object>::
reloadObject(const object_id_type& id, object_ptr object) {
	return reload(id);
}

template <class Final, class Object>
bool ActiveObjectManager<Final, Object>::
setupSlave(const object_id_type& id, object_ptr object) {
	Cell* cell = cell_expired_map_.get(id);
	if (!cell) {
		return false;
	}

	return true;
}

template <class Final, class Object>
bool ActiveObjectManager<Final, Object>::
setupSync(const rpc::condition_expr_type& expr) {
	if (ActiveObjectTraits<object_type>::SYNC_NAME) {
		ZCLOG(ERROR) << "SYNC_NAME was not set" << End;
		return false;
	}

	rpc::data_sync_handler_type handler(bind(&ActiveObjectManager::onSlaveSync, this, _1, _2),
			this->template getThreadIndex());

	return RPC.registerDataSyncHandler(ActiveObjectTraits<object_type>::SYNC_NAME, handler, expr);
}

template <class Final, class Object>
void ActiveObjectManager<Final, Object>::
onSlaveSync(rpc::key_type key, rpc::oarchiver_type params) {
	object_id_type id = 0;
	if (!(params["*id"] & id)) {
		ZCLOG(ERROR) << "Cant find id in Sync data" << End;
		return ;
	}

	object_ptr object;
	Cell* cell = cell_expired_map_.get(id);
	if (!cell) {
		object = create(id, 0);
	}
	else {
		object = cell->object;
	}

	object->setSlave();
	params & *object;

	object->onSync();
}

template <class Final, class Object>
typename ActiveObjectManager<Final, Object>::object_ptr ActiveObjectManager<Final, Object>::
get(object_id_type id, io::key_type provider) {
	Cell* cell = cell_expired_map_.get(id);
	if (!cell) {
		if (!prepare(id, provider)) {
			return null;
		}

		cell = cell_expired_map_.get(id);
		ZC_ASSERT( cell );
	}

	ZC_ASSERT( cell->object );
	cell->object->refresh();

	/**
	 * TODO: optimize the expired time
	 */
	cell_expired_map_.refresh(id, ActiveObjectTraits<object_type>::DefaultExpiredTick);
	return cell->object;
}

template <class Final, class Object>
typename ActiveObjectManager<Final, Object>::object_ptr ActiveObjectManager<Final, Object>::
create(object_id_type id, io::key_type provider_type) {
	this->template threadCheck();

	Cell c;
	c.id = id;

	if (provider_type) {
		c.provider = dp_manager::Instance().get(provider_type);
		if (!c.provider) {
			ZCLOG(ERROR) << "Didnt found Data Provider type:" << provider_type << End;
			return false;
		}
	}
	else {
		c.provider = NULL;
	}

	c.object = object_type::template create();

	c.object->setId(id);
	c.object->setManager(*this);

	if (!c.object->init()) {
		ZCLOG(ERROR) << "Object init failed" << End;
		return null;
	}

	cell_expired_map_.set(c.id, c, ActiveObjectTraits<object_type>::DefaultExpiredTick);
	return c.object;
}

template <class Final, class Object>
bool ActiveObjectManager<Final, Object>::
reload(object_id_type id) {
	Cell* cell = cell_expired_map_.get(id);
	if (!cell) {
		return false;
	}

	ZC_ASSERT( cell->object );

	if (!cell->provider) {
		ZCLOG(ERROR) << "Object don't have provider" << End;
		return false;
	}

	ConcurrentState::ptr fs = ConcurrentState::create([cell, id, this] (RuntimeContext::ptr ctx) {
		if (ctx && ctx->error) {
			ZCLOG(ERROR) << "Error:" << ctx->error.message << End;
			return ;
		}

		ZC_ASSERT( cell->object );
		cell->object->refresh();

		cell_expired_map_.refresh(id, ActiveObjectTraits<object_type>::DefaultExpiredTick);
	}, this->template getThreadIndex());

	if (!Concurrent::Instance().add([cell] () {
		if (!cell->provider->unserialize(cell->object, cell->id)) {
			threadContext().error.setError(error::NOT_FOUND);
			return ;
		}
	}, cell->provider->getThreadIndex(), fs))
		return false;

	fs->yeild();
	return true;
}

template <class Final, class Object>
bool ActiveObjectManager<Final, Object>::
prepare(object_id_type id, io::key_type provider_type, ConcurrentState::ptr state) {
	if (cell_expired_map_.get(id)) {
		return true;
	}

	typename CellGenerate::ptr cell = CellGenerate::create();
	cell->state = state;
	cell->id = id;
	cell->provider = dp_manager::Instance().get(provider_type);

	if (!cell->provider) {
		ZCLOG(ERROR) << "Didn't found Data Provider type:" << provider_type << End;
		return false;
	}

	ConcurrentState::ptr fs = ConcurrentState::create(
			bind(&ActiveObjectManager::setupObject, this, cell, _1),
			this->template getThreadIndex());

	cell->object = object_type::template create();

	if (state) {
		state->listen();
	}

	if (! Concurrent::Instance().add([cell] () {
		if (!cell->provider->unserialize(cell->object, cell->id)) {
			cell->object = object_type::Null;
			threadContext().error.setError(error::NOT_FOUND);
			return ;
		}
	}, cell->provider->getThreadIndex(), fs) ) return false;

	fs->yeild();
	return !fs->isError();
}

template <class Final, class Object>
void ActiveObjectManager<Final, Object>::
save(object_id_type id, ConcurrentState::ptr state) {
	goThread(this->template getThreadIndex(), [id, state, this] () -> void {
		Cell* it = cell_expired_map_.get(id);
		if (!it) {
			ZCLOG(ERROR) << "Id:" << id << " not found" << End;
			return ;
		}

		Cell& c = *it;
		ZC_ASSERT( c.object );

		if (!c.provider) {
			ZCLOG(ERROR) << "Object don't have provider" << End;
			return ;
		}

		dp_ptr dp = c.provider;
		object_ptr object = c.object;

		Concurrent::Instance().add([dp, object, id] () {

			if (!dp->serialize(object, id)) {
				ZCLOG(ERROR) << "data provider serialize failed" << End;
//				context().error.setError(error::SERIALIZE_FAILED);
			}

		}, dp->getThreadIndex(), state);
	});

	if (state)
		state->yeild();
}

template <class Final, class Object>
void ActiveObjectManager<Final, Object>::
save(object_ptr object, ConcurrentState::ptr state) {
	save(object->getId(), state);
}

template <class Final, class Object>
void ActiveObjectManager<Final, Object>::
setupObject(typename CellGenerate::ptr cell, RuntimeContext::ptr ctx) {
	this->template threadCheck();

	ZC_ASSERT( cell );

	do {
	if (ctx && ctx->error) {
		ZCLOG(ERROR) << "Error:" << ctx->error.message << End;
		break ;
	}

	if (!cell->object) {
		ZCLOG(ERROR) << "cell ptr is null" << End;
		break ;
	}

	cell->object->setId(cell->id);
	cell->object->setManager(*this);

	if (!cell->object->init()) {
		cell->object = object_type::Null;
		ZCLOG(ERROR) << "Object init failed" << End;
		break ;
	}

	/**
	 * add the cell to the map, would visit it via
	 * Instance().get(id)
	 */
	cell_expired_map_.set(cell->id, (Cell )(*cell),
			ActiveObjectTraits<object_type>::DefaultExpiredTick);

	}
	while(false);

	if (cell->state) {
		cell->state->complete();
	}
}

/**
template <class Final, class Object>
typename WorldObjectManager<Final, Object>::object_ptr
WorldObjectManager<Final, Object>::
fetch(const uuid_t& id, const io::key_type& provider_type) {
	Cell cell(id, provider_type);
	if (!cell.provider)
		return false;

	return cell.ptr;
}
*/

}}

#endif /* WORLDOBJECTMANAGERIMPL_HPP_ */
