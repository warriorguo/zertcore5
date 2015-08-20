/*
 * WorldObjectManagerImpl.hpp
 *
 *  Created on: 2014Äê11ÔÂ25ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_WORLDOBJECTMANAGERIMPL_HPP_
#define ZERTCORE_WORLDOBJECTMANAGERIMPL_HPP_

#include <pch.h>

#include <object/WorldObjectManager.h>
#include <concurrent/Concurrent.h>

namespace zertcore { namespace object {
using namespace zertcore::concurrent;
}}

namespace zertcore { namespace object {

template <class Final, class Object>
bool WorldObjectManager<Final, Object>::prepare(const uuid_t& id,
		const io::key_type& provider_type, ConcurrentState::ptr state) {

	Cell cell(id, provider_type);
	if (!cell.provider)
		return false;

	handler_type hr(bind(&WorldObjectManager<Final, Object>::setupObject, this, cell));
	hr.setThreadIndex(cell.provider->template getThreadIndex());

	return Concurrent::Instance().add(hr, state);
}

template <class Final, class Object>
void WorldObjectManager<Final, Object>::setupObject(Cell cell) {
	if (cell.provider) {
		Thread::getCurrentRunningContext().error.setError("");
		return ;
	}

	cell.ptr = object_type::template create();

	if (!cell.provider->unserialize(cell.ptr, cell.id) || !cell.ptr->init()) {
		cell.ptr = null;
		Thread::getCurrentRunningContext().error.setError("");
		return ;
	}
}

template <class Final, class Object>
typename WorldObjectManager<Final, Object>::object_ptr
WorldObjectManager<Final, Object>::fetch(const uuid_t& id,
			const io::key_type& provider_type) {
	Cell cell(id, provider_type);
	if (!cell.provider)
		return false;

	setupObject(cell);
	return cell.ptr;
}

}}

#endif /* WORLDOBJECTMANAGERIMPL_HPP_ */
