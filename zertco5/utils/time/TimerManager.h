/*
 * TimerManager.h
 *
 *  Created on: 2015年2月6日
 *      Author: Administrator
 */

#ifndef ZERTCORE_TIMERMANAGER_H_
#define ZERTCORE_TIMERMANAGER_H_

#include <pch.h>
#include <thread/ThreadSingleton.h>

#include "Timer.h"

namespace zertcore { namespace time {
using namespace zertcore::concurrent;
}}

namespace zertcore { namespace time {

/**
 * TimerManager
 */
class TimerManager :
		public ThreadSingleton<TimerManager>
{
public:
	timer_list_key_type registerTimer(timer_ptr timer);

public:
	size_t update();

private:
	timer_sorted_list_type		timer_list_;
};

}}



#endif /* TIMERMANAGER_H_ */
