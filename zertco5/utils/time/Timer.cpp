/*
 * Timer.cpp
 *
 *  Created on: 2015年2月6日
 *      Author: Administrator
 */

#include "Timer.h"
#include "TimerManager.h"

#include <details.h>

namespace zertcore { namespace time_utils {

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
	spinlock_guard_type guard(lock_);
	if (is_registered_) {
		is_registered_ = false;
		list_key_->second = null;
	}
}

void Timer::
add() {
	spinlock_guard_type guard(lock_);
	ZC_ASSERT(!is_registered_);

	is_expired_ = false;
	is_registered_ = true;

	list_key_ = TimerManager::Instance().registerTimer(this);
}

void Timer::
reset() {
	remove();
	add();
}

bool Timer::
expired() {
	return is_expired_;
}

void Timer::
onExpired(const handler_type& handler) {
	expired_handler_ = handler;
}

bool Timer::
start(const counter_type& intval) {
	if (!intval)
		return false;

	interval_ = intval;

	reset();
	return true;
}

void Timer::
stop() {
	remove();
}

void Timer::
timeUp() {
	spinlock_guard_type guard(lock_);
	ZC_ASSERT(is_registered_);

	is_registered_ = false;
	is_expired_ = true;

	if (expired_handler_) {
		expired_handler_.setParams(interval_);
		expired_handler_.push();
	}
}

}}

