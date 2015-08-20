/*
 * ThreadHandlerSetDetails.hpp
 *
 *  Created on: 2015年6月4日
 *      Author: Administrator
 */

#ifndef ZERTCORE_THREAD_DETAILS_THREADHANDLERSETDETAILS_HPP_
#define ZERTCORE_THREAD_DETAILS_THREADHANDLERSETDETAILS_HPP_

#include "../ThreadHandlerSet.h"
#include "../ThreadPool.h"
#include "../ThreadHandler.h"
#include "../Thread.h"

namespace zertcore { namespace concurrent {

template <class Final>
void ThreadHandlerSet<Final>::
initHandler(const init_handler_type& handler) {
	ThreadPool::Instance().registerInitHandler(getThreadIndex(), handler);
}

template <class Final>
tid_type ThreadHandlerSet<Final>::
getThreadIndex() {
	return Thread::lazyTid(Final::THREAD_INDEX);
}

template <class Final>
template <typename Callable>
bool ThreadHandlerSet<Final>::
go(Callable caller) {
	return ::zertcore::goThread(getThreadIndex(), caller);
}

}}

#endif /* THREAD_DETAILS_THREADHANDLERSETDETAILS_HPP_ */
