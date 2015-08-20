/*
 * mem_test1.cpp
 *
 *  Created on: 2015年6月20日
 *      Author: Administrator
 */

#include <utils/time/TimeType.h>
#include <utils/buffer/details/MemoryAllocator.h>

using namespace zertcore;
using namespace zertcore::utils;

#define TEST_SIZE					128

static std::list<void *> total_list;

static MemoryAllocator<TEST_SIZE, 10000> allocator;

#define TEST 1

#if TEST == 1
inline void *alloc(ulong size) {
	return malloc(size);//allocator.allocate();
}

inline void dealloc(void* mem) {
	free(mem); //allocator.deallocate(mem);
}
#elif TEST == 2
inline void *alloc(ulong size) {
	return allocator.allocate();
}

inline void dealloc(void* mem) {
	allocator.deallocate(mem);
}
#else
inline void *alloc(ulong size) {
	return nullptr;
}

inline void dealloc(void* mem) {
}
#endif

void round() {
	static void* rand_list[100000];
	long index = 0;

	for (long i = 0; i < 100000; ++i) {
		void* mem = alloc(TEST_SIZE);
//		ZC_ASSERT(mem);

		if (rand() % 2) {
			rand_list[index++] = mem;
		}
		else {
			total_list.push_back(mem);
		}
	}

	std::random_shuffle(&rand_list[0], &rand_list[index]);

	for (auto it = &rand_list[0]; it != &rand_list[index]; ++it) {
		dealloc(*it);
	}
}

int main() {
	srand(::time(NULL));
	allocator.init();

	time_type begin(Now);

	for (long i = 0; i < 100; ++i)
		round();

	for (auto it = total_list.begin(); it != total_list.end(); ++it) {
		dealloc(*it);
	}

	time_type end(Now);

	::printf("take time:%f\n", end.value - begin.value);
	return 0;
}

