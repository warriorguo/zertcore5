/*
 * Concurrent.cpp
 *
 *  Created on: 2014Äê10ÔÂ13ÈÕ
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

