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
#include "rpc/config.h"

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
	bool globalInit();

public:
	/**
	 * Add a handler to run asynchronously, set the context when it finished running.
	 * return an Key
	 */

	bool add(const handler_type& handler, const thread_ids_flag_type& flags, ConcurrentState::ptr state);
	bool add(const handler_type& handler, const thread_ids_flag_type& flags);

#ifdef ZC_ENABLE_THREADHANDLER_MULTICAST
	bool add(const handler_type& handler, const tid_type& index, ConcurrentState::ptr state);
	bool add(const handler_type& handler, const tid_type& index);
#endif

	template <typename Handler>
	bool add(const ThreadHandler<Handler>& handler, ConcurrentState::ptr state) {
		if (!state) return false;

		state->listen();

		task_type task(handler, handler.getThreadIndex());
		task.state = state;

		return ThreadPool::Instance().push(task);
	}

	bool add(const rpc::key_type& key, const rpc::iarchiver_type& params);
	bool add(const rpc::key_type& key, const rpc::iarchiver_type& params, ConcurrentState::ptr state);
	bool add(const rpc::key_type& key, const rpc::iarchiver_type& params, rpc::rpc_callback_type handler,
			ConcurrentState::ptr state);

public:
};

}}


#endif /* CONCURRENT_H_ */
