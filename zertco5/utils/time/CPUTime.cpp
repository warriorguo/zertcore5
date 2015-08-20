/*
 * CPUTime.cpp
 *
 *  Created on: 2015Äê1ÔÂ21ÈÕ
 *      Author: Administrator
 */

#include "CPUTime.h"

namespace zertcore { namespace time {

CPUTime operator + (const CPUTime& t1, const CPUTime& t2) {
	CPUTime t(t1); t += t2;
	return t;
}
CPUTime operator - (const CPUTime& t1, const CPUTime& t2) {
	CPUTime t(t1); t -= t2;
	return t;
}

}}

namespace zertcore { namespace time {
CPUTime::counter_type			CPUTime::counter_;

bool CPUTime::init() {
	counter_.start();
	return true;
}

CPUTime::CPUTime() : clock_(0) {
	;
}

CPUTime::CPUTime(const CPUTime& ct) : clock_(ct.clock_) {
	;
}

CPUTime::CPUTime(const __NOW& _) : clock_(0) {
	getTime();
}

const CPUTime& CPUTime::getTime() const {
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

bool CPUTime::operator == (const CPUTime& ct) {
	return clock_ == ct.clock_;
}
bool CPUTime::operator <= (const CPUTime& ct) {
	return clock_ <= ct.clock_;
}
bool CPUTime::operator >= (const CPUTime& ct) {
	return clock_ >= ct.clock_;
}
bool CPUTime::operator < (const CPUTime& ct) {
	return clock_ < ct.clock_;
}
bool CPUTime::operator > (const CPUTime& ct) {
	return clock_ > ct.clock_;
}

const CPUTime& CPUTime::operator -= (const CPUTime& ct) {
	if (clock_ >= ct.clock_)
		clock_ -= ct.clock_;
	else
		clock_ = 0;

	return *this;
}
const CPUTime& CPUTime::operator += (const CPUTime& ct) {
	clock_ += ct.clock_;
	return *this;
}

CPUTime::operator bool() const {
	return clock_ > 0;
}

}}
