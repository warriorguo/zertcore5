/*
 * Performance.h
 *
 *  Created on: 2015年3月5日
 *      Author: Administrator
 */

#ifndef UTILS_PERF_PERFORMANCE_H_
#define UTILS_PERF_PERFORMANCE_H_

#include <pch.h>
#include <utils/types.h>
#include <utils/time/TimeType.h>
#include <utils/Singleton.h>
#include <thread/ThreadSingleton.h>

namespace zertcore { namespace utils {
using namespace zertcore::concurrent;
}}

namespace zertcore { namespace utils {

/**
 * PerformanceManager
 */
class PerformanceManager :
		public ThreadSingleton<PerformanceManager>
{
public:
	typedef string							mod_type;

	struct Cell
	{
		u32						times{0};
		time_type				total_cost;
	};

	typedef unordered_map<mod_type, Cell>	perf_map_type;

public:
	virtual void init();

public:
	void add(const mod_type& mod, const time_type& cost);
	time_type getAverageCost(const mod_type& mod);

private:
	// Called in other thread
	size_t logData();

private:
	spinlock_type				lock_;
	perf_map_type				perf_map_;
};

/**
 * PerformanceSprite
 */
class PerformanceSprite : noncopyable
{
public:
	explicit PerformanceSprite(const string& mod) : begin_(Now), mod_(mod) {}
	~PerformanceSprite() {
		time_type end(Now);
		PerformanceManager::Instance().add(mod_, end - begin_);
	}

private:
	time_type					begin_;
	string						mod_;
};

}}

#define LOG_PERF_KEY(key)		::zertcore::utils::PerformanceSprite __perf_v_(key);
#define LOG_PERF()				LOG_PERF(__FUNCTION__)

#endif /* UTILS_PERF_PERFORMANCE_H_ */
