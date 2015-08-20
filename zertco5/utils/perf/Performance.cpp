/*
 * Performance.cpp
 *
 *  Created on: 2015年3月5日
 *      Author: Administrator
 */
#include "Performance.h"

#include <thread/ThreadPool.h>

namespace zertcore { namespace utils {

void PerformanceManager::
init() {
	/**
	 * make it to slow log method way
	 *
	ZC_ASSERT(ThreadPool::Instance().registerExclusiveHandler(
			bind(&PerformanceManager::logData, this)));
	*/
}

void PerformanceManager::
add(const mod_type& mod, const time_type& cost) {
//	spinlock_guard_type guard(lock_);

	Cell& cell = perf_map_[mod];
	cell.times++;
	cell.total_cost += cost;
}

time_type PerformanceManager::
getAverageCost(const mod_type& mod) {
	perf_map_type::const_iterator it = perf_map_.find(mod);
	if (it == perf_map_.end())
		return 0;

	const Cell& cell = it->second;
	return cell.total_cost.value / cell.times;
}

size_t PerformanceManager::
logData() {
	do {
		spinlock_guard_type guard(lock_);

		//save the data
	}
	while(false);

	usleep(1000);
	return 0;
}

}}


