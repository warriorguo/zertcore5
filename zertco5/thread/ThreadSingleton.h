/*
 * ThreadSingleton.h
 *
 *  Created on: 2015��1��5��
 *      Author: Administrator
 */

#ifndef ZERTCORE_THREADSINGLETON_H_
#define ZERTCORE_THREADSINGLETON_H_

#include <pch.h>
#include <utils/types.h>

#include <object/ObjectBase.h>

#include "Thread.h"
#include "ThreadLocal.h"
#include "ThreadHandlerSet.h"

namespace zertcore { namespace concurrent {
using namespace zertcore::object;
}}

namespace zertcore { namespace concurrent {

template <typename Final, bool EnableMap = true>
class ThreadSingleton :
		public ThreadHandlerSet<Final>,
		noncopyable
{
public:
	typedef vector<Final *>					thread_map_type;

public:
	virtual ~ThreadSingleton() {}

public:
	virtual void init() {}
	virtual tid_type getThreadIndex() const final {return Thread::getCurrentTid();}

public:
	/**
	 * the Instance() return the unique instance of this Thread!
	 * with the tid would get the instance of other instance
	 */
	static Final& Instance();
	static Final& Instance(tid_type tid);

public:
	virtual bool deinit();

private:
	static void __initInstance();

private:
	static ThreadLocal<Final *>	p_instance_;

private:
	static pthread_once_t		ponce_;
	static thread_map_type		thread_map_;
};

}}


#endif /* THREADSINGLETON_H_ */
