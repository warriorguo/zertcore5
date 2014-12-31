/*
 * Runtime.cpp
 *
 *  Created on: 2014Äê11ÔÂ12ÈÕ
 *      Author: Administrator
 */

#include <core/Runtime.h>

namespace zertcore{ namespace core{

bool Runtime::init() {
	return true;
}

size_t Runtime::update() {
	time_type now;
	now.getTime();

	size_t size = 0;
	for (updater_list_type::iterator it = updater_list_.begin();
			it != updater_list_.end(); ) {
		updater_list_type::iterator hit = it; ++it;
		if (time_type(hit->first) > now) {
			break;
		}

		(hit->second)();
		size++;

		updater_list_.erase(hit);
	}

	for (persistent_updater_list_type::iterator it = persistent_updater_list_.begin();
			it != persistent_updater_list_.end(); ) {
		persistent_updater_list_type::iterator hit = it; ++it;
		size += (*hit)();
	}

	return size;
}

}}
