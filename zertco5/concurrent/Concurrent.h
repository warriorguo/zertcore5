/*
 * Concurrent.h
 *
 *  Created on: 2014��10��13��
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
	bool add(const handler_type& handler, const thread_ids_flag_type& flags, ConcurrentState::ptr state);
	bool add(const handler_type& handler, const thread_ids_flag_type& flags);

	bool add(const handler_type& handler, const tid_type& index, ConcurrentState::ptr state);
	bool add(const handler_type& handler, const tid_type& index);

public:
};

}}


#endif /* CONCURRENT_H_ */
