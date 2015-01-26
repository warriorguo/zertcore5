/*
 * Thread.h
 *
 *  Created on: 2014Äê12ÔÂ23ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_THREAD_H_
#define ZERTCORE_THREAD_H_

#include <pch.h>
#include <utils/types.h>

#include <object/PoolObject.h>

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
	typedef ObjectTraits<Thread>::ptr		ptr;

public:
	explicit Thread(const tid_type& tid);
	virtual ~Thread();

public:
	static void setup() {
	}

public:
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
	static void setCurrentRunningContext(const RunningContext& context);
	static RunningContext& getCurrentRunningContext();
	/**
	 * get current tid
	 */
	static tid_type getCurrentTid();
	/**
	 * Cant guarantee its the other thread.
	 */
	static tid_type fetchOneOtherTid();

private:
	tid_type					tid_;
	ZC_TO_STRING(
		"TID" << tid_
	);

private:
	static ThreadLocal<tid_type>
								thread_index_;
	static ThreadLocal<RunningContext>
								thread_context_;
};

} /* namespace concurrent */ } /* namespace zertcore */

namespace zertcore {

}

#define ZC_ASSERT_RUN_IN_SAME_THREAD()		do{\
	static ::zertcore::tid_type __tid = ::zertcore::concurrent::Thread::getCurrentTid();\
	ZC_ASSERT(__tid == ::zertcore::concurrent::Thread::getCurrentTid());\
}while(false);

#endif /* THREAD_H_ */
