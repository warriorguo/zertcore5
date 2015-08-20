/*
 * Ticker.h
 *
 *  Created on: 2015年4月30日
 *      Author: Administrator
 */

#ifndef ZERTCORE_CONCURRENT_TICK_TICKER_H_
#define ZERTCORE_CONCURRENT_TICK_TICKER_H_

#include <pch.h>
#include <utils/types.h>
#include <utils/Singleton.h>
#include <utils/time/Timer.h>

#include "config.h"
#include "../Concurrent.h"

namespace zertcore { namespace concurrent { namespace update {
using namespace zertcore::utils;
}}}

namespace zertcore { namespace concurrent { namespace update {

/**
 * UpdateManager
 */
class UpdateServer :
		public Singleton<UpdateServer>
{
public:
	UpdateServer();

public:
	bool globalInit(const UpdateConfig& uc);

public:
	bool start();
	bool tickOnce();

public:
	void setInterval(const tick_type& interval) {
		interval_ = interval;
	}
	tick_type getInterval() const {
		return interval_;
	}

public:
	void printStatus();

private:
	void cbTimer(const tick_type& tick);
	void cbTick(RuntimeContext::ptr);

private:
	ConcurrentState::ptr		state_;
	u32							counter_;

private:
	tick_type					interval_;
	timer_type					timer_;
};

}}}


#endif /* CONCURRENT_TICK_TICKER_H_ */
