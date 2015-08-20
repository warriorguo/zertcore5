/*
 * SpinLock.h
 *
 *  Created on: 2015年4月22日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_SPINLOCK_SPINLOCK_H_
#define ZERTCORE_UTILS_SPINLOCK_SPINLOCK_H_

#include <pch.h>

namespace zertcore { namespace utils {

/**
 * LockGuard
 */
template <class LockType>
class LockGuard : noncopyable
{
public:
	typedef LockType						lock_type;

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
	int							v_;
};

}}


#endif /* UTILS_SPINLOCK_SPINLOCK_H_ */
