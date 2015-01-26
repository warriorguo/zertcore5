/*
 * ThreadSingleton.h
 *
 *  Created on: 2015Äê1ÔÂ5ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_THREADSINGLETON_H_
#define ZERTCORE_THREADSINGLETON_H_

#include <pch.h>
#include <utils/types.h>

#include <object/ObjectBase.h>

#include "ThreadLocal.h"

namespace zertcore { namespace concurrent {
using namespace zertcore::object;
}}

namespace zertcore { namespace concurrent {

template <typename Final>
class ThreadSingleton : noncopyable
{
public:
	virtual ~ThreadSingleton() {}

public:
	static inline Final& Instance() {
		Final* ins = p_instance_.load();
		if (!ins) {
			p_instance_.load() = ins = new Final;
		}

		return *ins;
	}

public:
	virtual bool deinit() {
		Final* ins = p_instance_.load();
		if (ins) {
			delete ins;
			p_instance_.load() = null;

			return true;
		}

		return false;
	}

private:
	static ThreadLocal<Final *>	p_instance_;
};

template <class Final>
ThreadLocal<Final*>				ThreadSingleton<Final>::p_instance_;

}}


#endif /* THREADSINGLETON_H_ */
