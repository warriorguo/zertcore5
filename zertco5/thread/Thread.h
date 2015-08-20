/*
 * Thread.h
 *
 *  Created on: 2014��12��23��
 *      Author: Administrator
 */

#ifndef ZERTCORE_THREAD_H_
#define ZERTCORE_THREAD_H_

#include <pch.h>
#include <utils/types.h>

#include <object/PoolObject.h>
#include <core/RuntimeContext.h>
#include <concurrent/ConcurrentState.h>

#include "ThreadLocal.h"

namespace zertcore {namespace concurrent {
using namespace zertcore::object;

class Thread;
}}

namespace zertcore { namespace object {
using namespace zertcore::concurrent;

/**
 * Specifier for ObjectTraits
 */
template <>
struct ObjectTraits<Thread> : public _Traits
{
	typedef Thread*							ptr;
};

}}

namespace zertcore {namespace concurrent {namespace details{

/**
 * __startThreadCallback
 */
void* __startThreadCallback(void *);

}}}

namespace zertcore {namespace concurrent {

/**
 * Thread
 */
class Thread :
		public PoolObject<Thread>
{
public:
	explicit Thread(const tid_type& tid);
	virtual ~Thread();

public:
	static void setup();
	static bool isSetup() {return is_setup_;}

public:
	void join();
	/**
	 * Init heandlers run firstly
	 */
	void init();
	/**
	 * Run
	 */
	void startRun();
public:
	/**
	 * set the running context
	 */
	static void setupCurrentRuntimeContext();
	static RuntimeContext& getCurrentRuntimeContext();
	/**
	 * get current tid
	 */
	static tid_type getCurrentTid();
	/**
	 * Cant guarantee its the other thread.
	 */
	static tid_type fetchOneOtherTid();

	/**
	 *
	 */
	static ConcurrentState::ptr getCurrentConcurrentState();
	static void setCurrentConcurrentState(ConcurrentState::ptr);

public:
	static tid_type lazyTid(const tid_type& tid);

private:
	pthread_t					pid_;
	tid_type					tid_;
	ZC_TO_STRING(
		"tid" << tid_
	);

private:
	static ThreadLocal<tid_type>
								thread_index_;
	static ThreadLocal<RuntimeContext>
								thread_context_;
	static ThreadLocal<ConcurrentState::ptr>
								thread_state_;

private:
	static bool					is_setup_;
};

} /* namespace concurrent */ } /* namespace zertcore */

namespace zertcore {

RuntimeContext& threadContext();
concurrent::ConcurrentState::ptr concurrentState();

}

#define ZC_ASSERT_RUN_IN_SAME_THREAD()		do{\
	static ::zertcore::tid_type __tid = ::zertcore::concurrent::Thread::getCurrentTid();\
	ZC_ASSERT(__tid == ::zertcore::concurrent::Thread::getCurrentTid());\
}while(false);

#endif /* THREAD_H_ */
