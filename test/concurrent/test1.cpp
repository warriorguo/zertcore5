/*
 * test1.cpp
 *
 *  Created on: 2014��10��13��
 *      Author: Administrator
 */

#include <concurrent/Concurrent.h>

using namespace zertcore::concurrent;

int main() {

	ConcurrentConfig config;
	config.thread_nums = 2;

	Concurrent::Instance().init(config);
	Concurrent::Instance().add([] (RunningContext& text) -> void {
		;
	});

	return 0;
}

