/*
 * CPUTime.cpp
 *
 *  Created on: 2015��1��21��
 *      Author: Administrator
 */

#include "Tick.h"
#include "TimeType.h"

namespace zertcore { namespace time_utils {

Tick operator + (const Tick& t1, const Tick& t2) {
	Tick t(t1); t += t2;
	return t;
}
Tick operator - (const Tick& t1, const Tick& t2) {
	Tick t(t1); t -= t2;
	return t;
}

}}

namespace zertcore { namespace time_utils {
Tick::counter_type				Tick::counter_;

bool Tick::init() {
	counter_.start();
	return true;
}

Tick::Tick() : clock_(0) {
	;
}

Tick::Tick(const Tick& ct) : clock_(ct.clock_) {
	;
}

Tick::Tick(const __NOW& _) : clock_(0) {
	getTime();
}

Tick::Tick(const value_type& c) : clock_(c * (SCALE_RATIO / 1000)) {
	;
}

Tick::operator TimeType() const {
	TimeType tt;
	tt.value = (TimeType::type)clock_ / (SCALE_RATIO / 1000);

	return tt;
}

const Tick& Tick::getTime() const {
	ZC_DEBUG_ASSERT(!counter_.is_stopped());

	timer::cpu_times e = counter_.elapsed();
	clock_ = e.wall;

	return *this;
}

/**
void CPUTime::start() {
	counter_.start();
}
void CPUTime::stop() {
	counter_.stop();
}
void CPUTime::resume() {
	counter_.resume();
}

bool CPUTime::isStopped() const {
	return counter_.is_stopped();
}

CPUTime::value_type CPUTime::elapsed() const {
	timer::cpu_times e = counter_.elapsed();
	return e.system + e.user;
}
*/

bool Tick::operator == (const Tick& ct) const {
	return clock_ == ct.clock_;
}
bool Tick::operator <= (const Tick& ct) const {
	return clock_ <= ct.clock_;
}
bool Tick::operator >= (const Tick& ct) const {
	return clock_ >= ct.clock_;
}
bool Tick::operator < (const Tick& ct) const {
	return clock_ < ct.clock_;
}
bool Tick::operator > (const Tick& ct) const {
	return clock_ > ct.clock_;
}

const Tick& Tick::operator -= (const Tick& ct) {
	if (clock_ >= ct.clock_)
		clock_ -= ct.clock_;
	else
		clock_ = 0;

	return *this;
}
const Tick& Tick::operator += (const Tick& ct) {
	clock_ += ct.clock_;
	return *this;
}

Tick::operator bool() const {
	return clock_ > 0;
}

}}
