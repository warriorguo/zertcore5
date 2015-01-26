/*
 * Timer.h
 *
 *  Created on: 2015Äê1ÔÂ21ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_TIME_TIMER_H_
#define ZERTCORE_TIME_TIMER_H_

#include <pch.h>
#include "CPUTime.h"

namespace zertcore { namespace time {

/**
 * Timer
 */
class Timer : noncopyable
{
public:
	typedef CPUTime							time_type;
	typedef time_type::value_type			invterval_type;

public:
	Timer();
	virtual ~Timer();

public:
	bool expired();

public:
	void start();
	void stop();

private:
	invterval_type				interval_;
};

}}


#endif /* TIMER_H_ */
