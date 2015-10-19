/*
 * Concurrent.cpp
 *
 *  Created on: 2014��10��13��
 *      Author: Administrator
 */

#include <pch.h>
#include <thread/ThreadPool.h>
#include "Concurrent.h"
#include "rpc/RPCManager.h"

#include <details.h>

namespace zertcore { namespace concurrent {

bool Concurrent::
globalInit() {
	return true;
}

bool Concurrent::
add(const handler_type& handler, const thread_ids_flag_type& flags, ConcurrentState::ptr state) {
	if (state)
		state->listen();

	task_type task(handler, flags);
	task.state = state;

	return ThreadPool::Instance().push(task);
}
bool Concurrent::
add(const handler_type& handler, const thread_ids_flag_type& flags) {
	task_type task(handler, flags);
	return ThreadPool::Instance().push(task);
}

#ifdef ZC_ENABLE_THREADHANDLER_MULTICAST
bool Concurrent::
add(const handler_type& handler, const tid_type& tid, ConcurrentState::ptr state) {
	if (!state)
		return false;

	if (!ThreadPool::Instance().isUsableTid(tid)) {
		return false;
	}

	state->listen();
	thread_ids_flag_type flags(tid + 1);
	flags.set(tid, true);

	task_type task(handler, flags);
	task.state = state;

	return ThreadPool::Instance().push(task);
}

bool Concurrent::
add(const handler_type& handler, const tid_type& tid) {
	if (!ThreadPool::Instance().isUsableTid(tid)) {
		return false;
	}

	thread_ids_flag_type flags(tid + 1);
	flags.set(tid, true);

	task_type task(handler, flags);

	return ThreadPool::Instance().push(task);
}

#endif

bool Concurrent::
add(const rpc::key_type& key, const rpc::iarchiver_type& params) {
	return rpc::RPCManager::Instance().asyncCall(key, params);
}

bool Concurrent::
add(const rpc::key_type& key, const rpc::iarchiver_type& params, ConcurrentState::ptr state) {
	return rpc::RPCManager::Instance().asyncCall(key, params, rpc::rpc_callback_type(), state);
}

bool Concurrent::
add(const rpc::key_type& key, const rpc::iarchiver_type& params,
		rpc::rpc_callback_type handler, ConcurrentState::ptr state) {
	return rpc::RPCManager::Instance().asyncCall(key, params, handler, state);
}

}}

