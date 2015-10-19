/*
 * TimerManager.cpp
 *
 *  Created on: 2015年2月6日
 *      Author: Administrator
 */

#include <core/Runtime.h>
#include <log/Log.h>

#include "TimerManager.h"

namespace zertcore { namespace time_utils {
using namespace zertcore::core;
}}

namespace zertcore { namespace time_utils {

void TimerManager::
init() {}

timer_list_key_type TimerManager::
registerTimer(timer_ptr timer) {
	ZC_ASSERT(timer);

	timer_type::counter_type now(Now);
	return timer_list_.insert(
			timer_sorted_list_type::value_type(now + timer->getInterval(), timer));
}

u32 TimerManager::
update() {
	u32 update_size = 0;
	timer_type::counter_type now(Now);

	for (auto it = timer_list_.begin(); it != timer_list_.end(); ) {
		auto hit(it++);

		if (now < hit->first)
			break;

		timer_ptr timer = hit->second;
		timer_list_.erase(hit);

		if (timer) {
			timer->timeUp();
		}

		++update_size;
	}

	return update_size;
}

}}

