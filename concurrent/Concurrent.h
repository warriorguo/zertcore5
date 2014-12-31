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

#include <concurrent/config.h>
#include <concurrent/thread/ThreadPool.h>
#include <concurrent/ConcurrentState.h>

namespace zertcore { namespace concurrent {
using namespace zertcore::utils;

typedef uint								key_type;
}}

namespace zertcore { namespace concurrent {

/**
 * Concurrent
 */
class Concurrent :
		public Singleton<Concurrent>
{
public:
	bool init(const ConcurrentConfig& config);

public:
	/**
	 * Add a handler to run asynchronously, set the context when it finished running.
	 * return an Key
	 */
	bool add(handler_type handler, ConcurrentState::ptr state);
	bool add(handler_type handler);

	bool add(const string& host, const u32& port);

public:
};

}}


#endif /* CONCURRENT_H_ */
