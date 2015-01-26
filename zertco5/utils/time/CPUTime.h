/*
 * CPUTime.h
 *
 *  Created on: 2015Äê1ÔÂ21ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_TIME_CPUTIME_H_
#define ZERTCORE_TIME_CPUTIME_H_

#include <pch.h>
#include <utils/types.h>
#include <object/ObjectBase.h>

namespace zertcore { namespace time {
using namespace zertcore::object;
}}

namespace zertcore { namespace time {

/**
 * CPUTime, mainly for time compare, dont log it as real time,
 * see TimeType(time_type)
 */
class CPUTime :
		public ObjectBase<CPUTime>
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
	CPUTime();
	CPUTime(const CPUTime&);
	CPUTime(const __NOW&);
/**
public:
	void start();
	void stop();
	void resume();

	bool isStopped() const;

	value_type elapsed() const;
*/
public:
	const CPUTime& getTime() const;

public:
	bool operator == (const CPUTime& ct);
	bool operator <= (const CPUTime& ct);
	bool operator >= (const CPUTime& ct);
	bool operator < (const CPUTime& ct);
	bool operator > (const CPUTime& ct);

	const CPUTime& operator -= (const CPUTime& ct);
	const CPUTime& operator += (const CPUTime& ct);

public:
	operator bool() const;

public:
	const value_type& value() const {return clock_;}

private:
	mutable value_type			clock_;

private:
	static counter_type			counter_;
};

/**
 * Helper operators
 */
CPUTime operator + (const CPUTime& t1, const CPUTime& t2);
CPUTime operator - (const CPUTime& t1, const CPUTime& t2);

}}

namespace zertcore {
typedef time::CPUTime						cpu_time_type;
}

#endif /* CPUTIME_H_ */
