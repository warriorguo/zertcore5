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
		const io::key_type& provider_type, handler_type handler) {

	Cell cell(id, provider_type);
	if (!cell.provider)
		return false;

	return Concurrent::Instance().add(
			bind(&WorldObjectManager<Final, Object>::setupObject, this, cell),
			handler, dp->getThreadIndex());
}

template <class Final, class Object>
void WorldObjectManager<Final, Object>::setupObject(Cell cell) {
	if (cell.provider) {
		cell.ptr = Object::create();

		if (!cell.provider->unserialize(cell.ptr, cell.id) || !cell.ptr->init()) {
			cell.ptr = null;
		}

	}
}

template <class Final, class Object>
typename WorldObjectManager<Final, Object>::object_ptr
WorldObjectManager<Final, Object>::fetch(const uuid_t& id,
			const io::key_type& provider_type) {
	Cell cell(id, provider_type);
	setupObject(cell);

	return cell.ptr;
}

}}

#endif /* WORLDOBJECTMANAGERIMPL_HPP_ */
