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

template <class Final, bool Flag>
ThreadLocal<Final*>				ThreadSingleton<Final, Flag>::p_instance_;

template <class Final, bool Flag>
pthread_once_t					ThreadSingleton<Final, Flag>::ponce_ = PTHREAD_ONCE_INIT;

template <class Final, bool Flag>
typename ThreadSingleton<Final, Flag>::thread_map_type
								ThreadSingleton<Final, Flag>::thread_map_;

}}

namespace zertcore { namespace concurrent {

template <typename Final, bool Flag>
Final& ThreadSingleton<Final, Flag>::
Instance() {
	Final* ins = p_instance_.load();
	if (!ins) {
		p_instance_.load() = ins = new Final;

		if (Flag) {
			ZC_ASSERT( ThreadPool::Instance().totalSize() );

			if (thread_map_.empty())
				pthread_once(&ponce_, &ThreadSingleton::__initInstance);

			tid_type tid = Thread::getCurrentTid() - 1;
			ZC_ASSERT(tid < thread_map_.size());

			thread_map_[tid] = ins;
		}

		ins->init();
	}

	return *ins;
}

template <typename Final, bool Flag>
Final& ThreadSingleton<Final, Flag>::
Instance(tid_type tid) {
	ZC_ASSERT(Flag);

	tid--;
	ZC_ASSERT(tid < thread_map_.size());
	ZC_ASSERT(thread_map_[tid]);

	return *thread_map_[tid];
}

template <typename Final, bool Flag>
bool ThreadSingleton<Final, Flag>::
deinit() {
	Final* ins = p_instance_.load();
	if (ins) {
		delete ins;
		p_instance_.load() = null;

		return true;
	}

	return false;
}

template <typename Final, bool Flag>
void ThreadSingleton<Final, Flag>::
__initInstance() {
	ZC_ASSERT(thread_map_.empty());
	thread_map_.resize(ThreadPool::Instance().totalSize(), nullptr);
}

}}


#endif /* THREAD_DETAILS_THREADSINGLETONIMPL_HPP_ */
