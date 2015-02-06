/*
 * TimerManager.cpp
 *
 *  Created on: 2015年2月6日
 *      Author: Administrator
 */

#include <core/Runtime.h>
#include "TimerManager.h"

namespace zertcore { namespace time {
using namespace zertcore::core;
}}

namespace zertcore { namespace time {

inline timer_list_key_type TimerManager::
registerTimer(timer_ptr timer) {
	ZC_ASSERT(timer);

	Timer::time_type now(Now);
	return timer_list_.insert(
			timer_sorted_list_type::value_type(
					now.value() + timer->getInterval(), timer));
}

size_t TimerManager::
update() {
	size_t update_size = 0;
	Timer::time_type now(Now);

	for (auto it = timer_list_.begin(); it != timer_list_.end(); ) {
		auto hit = it++;
		timer_ptr timer = hit->second;

		if (now.value() < hit->first)
			break;

		timer_list_.erase(hit);

		if (timer) {
			timer->timeUp();
		}

		++update_size;
	}

	return update_size;
}

}}

