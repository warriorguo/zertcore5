/*
 * Concurrent.cpp
 *
 *  Created on: 2014Äê10ÔÂ13ÈÕ
 *      Author: Administrator
 */

#include <concurrent/Concurrent.h>
#include <concurrent/thread/ThreadPool.h>

namespace zertcore { namespace concurrent {

bool Concurrent::
init(const ConcurrentConfig& config) {
	return ThreadPool::Instance().start(config.thread_nums);
}

bool Concurrent::
add(handler_type handler, ConcurrentState::ptr state) {
	state->listen();

	task_type task(handler);
	task.state = state;

	return ThreadPool::Instance().push(task);
}

bool Concurrent::
add(handler_type handler) {
	return ThreadPool::Instance().push(handler);
}

}}

