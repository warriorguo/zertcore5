/*
 * Timer.h
 *
 *  Created on: 2015��1��21��
 *      Author: Administrator
 */

#ifndef ZERTCORE_TIME_TIMER_H_
#define ZERTCORE_TIME_TIMER_H_

#include <pch.h>
#include <object/PoolObject.h>

#include "CPUTime.h"

namespace zertcore { namespace time {
using namespace zertcore::object;

class Timer;
typedef Timer*								timer_ptr;

// its NOT a optimal plan for the timer list since easing an element was too SLOW
typedef multimap<CPUTime::value_type, timer_ptr>
											timer_sorted_list_type;
typedef timer_sorted_list_type::iterator	timer_list_key_type;
}}

namespace zertcore { namespace time {

/**
 * Timer : count by ms
 */
class Timer :
		public PoolObject<Timer>
{
	friend class TimerManager;
public:
	typedef CPUTime							time_type;
	typedef time_type::value_type			interval_type;

	typedef function<void (const interval_type&)>
											handler_type;

public:
	Timer();
	virtual ~Timer();

public:
	void reset();
	bool isWorking() const {return is_registered_;}

public:
	bool expired();
	void onExpired(const handler_type& handler);

public:
	/**
	 * count by ms
	 */
	bool start(const interval_type& intval);
	void stop();

public:
	interval_type getInterval() const {return interval_;}

private:
	void remove();
	void add();

private:
	//handle by TimerManager
	void timeUp();

private:
	interval_type				interval_;
	handler_type				expired_handler_;

	bool						is_expired_;
	bool						is_registered_;
	timer_list_key_type			list_key_;

	ZC_TO_STRING("interval" << interval_ << "is_expired" << is_expired_ << "is_registered" << is_registered_);
};

}}


#endif /* TIMER_H_ */
