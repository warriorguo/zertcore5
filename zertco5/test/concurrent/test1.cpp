/*
 * test1.cpp
 *
 *  Created on: 2014Äê10ÔÂ13ÈÕ
 *      Author: Administrator
 */
#include <config/Config.h>
#include <core/Runtime.h>
#include <concurrent/Concurrent.h>

using namespace zertcore::concurrent;
using namespace zertcore;

int main() {
	config.concurrent.thread_nums = 3;

	RT.init([]() {
		ConcurrentState::ptr state = ConcurrentState::create(callback_type([&] (const RunningContext& rc) {
			if (rc.error)
				::printf("I am in cb : %s\n", rc.error.message.c_str());
			else
				::printf("I am in cb and no error\n");
		}));

		handler_type hr1([] () -> void {
			::printf("I am in hr1\n");
			sleep(1);
		});
		hr1.setThreadIndex(1);

		handler_type hr2([] () -> void {
			::printf("I am in hr2\n");
			Thread::getCurrentRunningContext().error.setError("HAHA");
		});
		hr2.setThreadIndex(1);

		Concurrent::Instance().add(hr2, state);
		Concurrent::Instance().add(hr1, state);
	});

	RT.run();
	return 0;
}

