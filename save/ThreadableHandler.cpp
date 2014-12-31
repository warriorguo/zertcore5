/*
 * ThreadableHandler.cpp
 *
 *  Created on: 2014Äê12ÔÂ9ÈÕ
 *      Author: Administrator
 */

#include <pch.h>

#include <concurrent/thread/ThreadableHandler.h>
#include <concurrent/Concurrent.h>

namespace zertcore { namespace concurrent {

ThreadableHandler& ThreadableHandler::operator= (const origin_handler_type& h) {
	handler = h;
	return *this;
}

bool ThreadableHandler::inhert(const ThreadableHandler& handler) {
	if (handler.context.error) {
		lock_guard<typeof(lock_)> guard(lock_);

		context.error = handler.context.error;
		return true;
	}

	return false;
}

bool ThreadableHandler::pushThread() {
	return Concurrent::Instance().add(*this);
}

}}

