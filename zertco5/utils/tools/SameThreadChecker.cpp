/*
 * SameThreadChecker.cpp
 *
 *  Created on: 2015年5月29日
 *      Author: Administrator
 */

#include "SameThreadChecker.h"
#include <thread/Thread.h>
#include <log/Log.h>

namespace zertcore { namespace utils {
using namespace zertcore::concurrent;
}}

namespace zertcore { namespace utils {

#ifdef DISABLE_THREAD_CHECKER

void SameThreadChecker::init() {}
void SameThreadChecker::check() const {}

#else

void SameThreadChecker::
init() {
	if (!Thread::isSetup()) {
		ZCLOG(FATAL) << "Thread was not setup" << End;
	}

	tid_ = Thread::getCurrentTid();
}

void SameThreadChecker::
check() const {
	tid_type current_tid = Thread::getCurrentTid();
	if (tid_ != current_tid) {
		ZCLOG(FATAL) << "Thread Checker failed: Should In " << tid_ << " Current In " << current_tid << End;
	}
}

#endif

}}

