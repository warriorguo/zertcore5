/*
 * SameThreadChecker.h
 *
 *  Created on: 2015年5月29日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_TOOLS_SAMETHREADCHECKER_H_
#define ZERTCORE_UTILS_TOOLS_SAMETHREADCHECKER_H_

#include <pch.h>
#include <utils/types.h>

namespace zertcore { namespace utils {

/**
 * SameThreadChecker
 */
class SameThreadChecker
{
public:
	void init();

public:
	void check() const;

private:
	tid_type					tid_;
};

}}

#define ZC_THREAD_CHECKER		private: \
	::zertcore:utils::SameThreadChecker __sthread_checker;
#define ZC_THREAD_CHECKER_INIT	do{__sthread_checker.init();}while(false);
#define ZC_THREAD_CHECKER_DO	do{__sthread_checker.check();}while(false);

#endif /* UTILS_TOOLS_SAMETHREADCHECKER_H_ */
