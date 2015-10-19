/*
 * CPUTime.h
 *
 *  Created on: 2015��1��21��
 *      Author: Administrator
 */

#ifndef ZERTCORE_TIME_CPUTIME_H_
#define ZERTCORE_TIME_CPUTIME_H_

#include <pch.h>
#include <utils/types.h>
#include <object/ObjectBase.h>

namespace zertcore { namespace time_utils {
using namespace zertcore::object;
}}

namespace zertcore { namespace time_utils {
/**
 *
 */
class TimeType;

/**
 * CPUTime, mainly for time compare, dont log it as real time,
 * see TimeType(time_type)
 */
class Tick :
		public ObjectBase<Tick>
{
public:
	typedef timer::nanosecond_type			value_type;
	typedef timer::cpu_timer				counter_type;

public:
	const static value_type					SCALE_RATIO = 1000000000;

public:
	static bool init();

	ZC_TO_STRING(clock_);

public:
	Tick();
	Tick(const value_type&);
	Tick(const Tick&);
	Tick(const __NOW&);
/**
public:
	void start();
	void stop();
	void resume();

	bool isStopped() const;

	value_type elapsed() const;
*/
public:
	operator TimeType() const;

public:
	const Tick& getTime() const;

public:
	bool operator == (const Tick& ct) const;
	bool operator <= (const Tick& ct) const;
	bool operator >= (const Tick& ct) const;
	bool operator < (const Tick& ct) const;
	bool operator > (const Tick& ct) const;

	const Tick& operator -= (const Tick& ct);
	const Tick& operator += (const Tick& ct);

public:
	operator bool() const;

public:
	const value_type& value() const {return clock_;}
	value_type& value() {return clock_;}

private:
	mutable value_type			clock_;

private:
	static counter_type			counter_;
};

/**
 * Helper operators
 */
Tick operator + (const Tick& t1, const Tick& t2);
Tick operator - (const Tick& t1, const Tick& t2);

}}

namespace zertcore {
typedef time_utils::Tick						tick_type;
}

#endif /* CPUTIME_H_ */
