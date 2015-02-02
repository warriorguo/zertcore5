/*
 * Concurrent.cpp
 *
 *  Created on: 2014��10��13��
 *      Author: Administrator
 */

#include <thread/ThreadPool.h>
#include "Concurrent.h"

namespace zertcore { namespace concurrent {

bool Concurrent::
init() {
	return true;
}

bool Concurrent::
add(const handler_type& handler, const thread_ids_flag_type& flags, ConcurrentState::ptr state) {
	if (!state) return false;

	state->listen();

	task_type task(handler, flags);
	task.state = state;
	task.flags = flags;

	return ThreadPool::Instance().push(task);
}
bool Concurrent::
add(const handler_type& handler, const thread_ids_flag_type& flags) {
	task_type task(handler, flags);
	return ThreadPool::Instance().push(task);
}

bool Concurrent::
add(const handler_type& handler, const tid_type& tid, ConcurrentState::ptr state) {
	if (!state) return false;
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

}}

