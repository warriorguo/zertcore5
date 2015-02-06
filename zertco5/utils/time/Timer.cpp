/*
 * Timer.cpp
 *
 *  Created on: 2015年2月6日
 *      Author: Administrator
 */

#include "Timer.h"
#include "TimerManager.h"

namespace zertcore { namespace time {

Timer::Timer() :
		interval_(0), is_expired_(false),
		is_registered_(false) {
	;
}

Timer::~Timer() {
	remove();
}

void Timer::
remove() {
	if (is_registered_) {
		is_registered_ = false;
		*list_key_ = null;
	}
}

void Timer::
add() {
	is_expired_ = false;
	is_registered_ = true;

	list_key_ = TimerManager::Instance().registerTimer(this);
}

inline void Timer::
reset() {
	remove();
	add();
}

inline bool Timer::
expired() {
	return is_expired_;
}

inline void Timer::
onExpired(const handler_type& handler) {
	expired_handler_ = handler;
}

inline bool Timer::
start(const interval_type& intval) {
	if (intval == 0)
		return false;

	interval_ = intval * (time_type::SCALE_RATIO / 1000);

	reset();
	return true;
}

inline void Timer::
stop() {
	remove();
}

inline void Timer::
timeUp() {
	ZC_ASSERT(is_registered_);

	is_registered_ = false;
	is_expired_ = true;

	if (expired_handler_)
		expired_handler_();
}

}}

