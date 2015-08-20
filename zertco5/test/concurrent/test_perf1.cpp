/*
 * test_perf1.cpp
 *
 *  Created on: 2015年6月22日
 *      Author: Administrator
 */

#include <zertcore.h>

using namespace zertcore;
using namespace zertcore::utils;
using namespace zertcore::concurrent;

#define TEST_TIMES							100000

void func() {
	for (uint i = 0; i < 10000; ++i);
}

int main() {
	/**
	size_t offset = 0;
	for (int i = 0; i < 10; ++i) {
		void* ptr1 = malloc(offset + 1);
		void* ptr2 = malloc(offset + 1);
		free(ptr1);
		free(ptr2);

		offset = (long long)ptr2 - (long long)ptr1;
		::printf("%lld\n", offset);
	}
	*/

	do {
	time_type begin(Now);
	for (u32 i = 0; i < TEST_TIMES; ++i) {
		func();
	}
	time_type end(Now);

	::printf("pure take time:%f\n", (end - begin).value);

	}while(false);

	do {
	time_type begin(Now);
	for (u32 i = 0; i < TEST_TIMES; ++i) {
		function<void ()> handler(&func);
		handler();
	}
	time_type end(Now);

	::printf("function take time:%f\n", (end - begin).value);

	}while(false);

	u32 cpu_count = System::CPUCoreCount();
	/**
	 * the CPU amount was 6,
	 * and its quite strange here!
	 * when thread_nums was 2, the execute time was about 1/2 of pure call (function way was the fastest)
	 * when its 3, the a little bit above 1/3 of pure call cost
	 * when its 4, JUST a little bit below 1/3 of pure call cost
	 * when its 5, 6, its almost the same to the solution with 4
	 */
	::printf("Cpu Count = %u\n", cpu_count);

	config.concurrent.thread_nums = cpu_count;
	RT.globalInit([]() {
		time_type begin(Now);
		ConcurrentState::ptr state = ConcurrentState::create([begin] (RuntimeContext::ptr) {
			time_type end(Now);
			::printf("thread handler take time:%f\n", (end - begin).value);

		});

		for (u32 i = 0; i < TEST_TIMES; ++i) {
//			ThreadHandler<void ()> handler(&func, Thread::lazyTid(i));
			Concurrent::Instance().add(&func, Thread::lazyTid(i), state);
		}
	});
	RT.run();

	return 0;
}


