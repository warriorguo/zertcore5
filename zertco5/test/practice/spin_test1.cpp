/*
 * spin_test1.cpp
 *
 *  Created on: 2015年6月23日
 *      Author: Administrator
 */

#include <cstdio>
#include <unistd.h>
#include <pthread.h>

#define X 1

namespace zertcore { namespace utils {

/**
 * LockGuard
 */
template <class LockType>
class LockGuard
{
public:
	typedef LockType				lock_type;

public:
	explicit LockGuard( lock_type & lock ) : lock_(lock) {
		lock_.lock();
	}
	~LockGuard() {
		lock_.unlock();
	}

private:
	lock_type&					lock_;
};

namespace details {
/**
 * yield
 */
inline static void yield(unsigned long long t) {
	if (t < 5) {}
	else if (t < 50) {usleep(0);}
	else {usleep(1);}
}

}


#if X == 1
class SpinLock
{
public:
	SpinLock() : v_(0) {}

public:
	bool try_lock() {
		int r = __sync_lock_test_and_set(&v_, 1);
		return r == 0;
	}

	void lock(){
		for(unsigned long long k = 0; !try_lock(); ++k) {
			details::yield( k );
		}
	}

	void unlock() {
		__sync_lock_release(&v_);
	}

private:
	int				v_;
};
#else

class SpinLock
{
public:

    pthread_mutex_t v_;

public:

    bool try_lock()
    {
        return pthread_mutex_trylock( &v_ ) == 0;
    }

    void lock()
    {
        pthread_mutex_lock( &v_ );
    }

    void unlock()
    {
        pthread_mutex_unlock( &v_ );
    }
};

#endif

class T
{
public:
	void f1() {
		LockGuard<SpinLock> guard(lock_);
		counter_++;
	}
	void f2() {
		LockGuard<SpinLock> guard(lock_);
		counter_++;
	}
	void r() {
		::printf("counter:%ld\n", counter_);
	}

private:
	volatile long				counter_{0};
	SpinLock					lock_;
};

}}

using namespace zertcore::utils;

#define LOOP_TIMES				10000000

T t;

void* thread_1(void*) {
	for (long i = 0; i < LOOP_TIMES; ++i)
		t.f1();
	return nullptr;
}

void* thread_2(void*) {
	for (long i = 0; i < LOOP_TIMES; ++i)
		t.f2();
	return nullptr;
}

int main() {
	pthread_t pid1 = 0, pid2 = 0;

	pthread_create(&pid1, nullptr, &thread_1, nullptr);
	pthread_create(&pid2, nullptr, &thread_2, nullptr);

	pthread_join(pid1, nullptr);
	pthread_join(pid2, nullptr);

	t.r();
	return 0;
}

