/*
 * Concurrent.h
 *
 *  Created on: 2014Äê10ÔÂ13ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_CONCURRENT_H_
#define ZERTCORE_CONCURRENT_H_

#include <pch.h>
#include <utils/Singleton.h>
#include <thread/ThreadPool.h>

#include "config.h"
#include "ConcurrentState.h"

namespace zertcore { namespace concurrent {
using namespace zertcore::utils;
}}

namespace zertcore { namespace concurrent {

/**
 * Concurrent
 */
class Concurrent :
		public Singleton<Concurrent>
{
public:
	bool init();

public:
	/**
	 * Add a handler to run asynchronously, set the context when it finished running.
	 * return an Key
	 */
	bool add(handler_type handler, ConcurrentState::ptr state);
	bool add(handler_type handler);

public:
};

}}


#endif /* CONCURRENT_H_ */
