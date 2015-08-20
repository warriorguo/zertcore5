/*
 * ThreadSingletonImpl.hpp
 *
 *  Created on: 2015年7月31日
 *      Author: Administrator
 */

#ifndef ZERTCORE_THREAD_DETAILS_THREADSINGLETONIMPL_HPP_
#define ZERTCORE_THREAD_DETAILS_THREADSINGLETONIMPL_HPP_

#include "../ThreadSingleton.h"
#include "../ThreadPool.h"

namespace zertcore { namespace concurrent {

template <class Final>
ThreadLocal<Final*>				ThreadSingleton<Final>::p_instance_;

template <class Final>
pthread_once_t					ThreadSingleton<Final>::ponce_ = PTHREAD_ONCE_INIT;

template <class Final>
typename ThreadSingleton<Final>::thread_map_type
								ThreadSingleton<Final>::thread_map_;

}}

namespace zertcore { namespace concurrent {

template <typename Final>
Final& ThreadSingleton<Final>::
Instance() {
	Final* ins = p_instance_.load();
	if (!ins) {
		p_instance_.load() = ins = new Final;
		ZC_ASSERT( ThreadPool::Instance().totalSize() );

		if (thread_map_.empty())
			pthread_once(&ponce_, &ThreadSingleton::__initInstance);

		tid_type tid = Thread::getCurrentTid() - 1;
		ZC_ASSERT(tid < thread_map_.size());

		thread_map_[tid] = ins;

		ins->init();
	}

	return *ins;
}

template <typename Final>
Final& ThreadSingleton<Final>::
Instance(tid_type tid) {
	tid--;
	ZC_ASSERT(tid < thread_map_.size());
	ZC_ASSERT(thread_map_[tid]);

	return *thread_map_[tid];
}

template <typename Final>
bool ThreadSingleton<Final>::
deinit() {
	Final* ins = p_instance_.load();
	if (ins) {
		delete ins;
		p_instance_.load() = null;

		return true;
	}

	return false;
}

template <typename Final>
void ThreadSingleton<Final>::
__initInstance() {
	ZC_ASSERT(thread_map_.empty());
	thread_map_.resize(ThreadPool::Instance().totalSize(), nullptr);
}

}}


#endif /* THREAD_DETAILS_THREADSINGLETONIMPL_HPP_ */
