/*
 * RWLock.hpp
 *
 *  Created on: 2014��11��6��
 *      Author: Administrator
 */

#ifndef ZERTCORE_RWLOCK_HPP_
#define ZERTCORE_RWLOCK_HPP_

#include <pch.h>
#include <utils/types.h>

#include <object/PoolObject.h>

namespace zertcore{ namespace concurrent{ namespace details{
using namespace zertcore::object;
}}}

namespace zertcore{ namespace concurrent{ namespace details{

/**
 * RWLockContainer
 */
template <class STL_Container>
class RWLock : public PoolObject<RWLock<STL_Container> >
{
public:
	typedef STL_Container					container_type;
	typedef SMART_PTR(container_type)
											container_ptr;
	typedef typename container_type::value_type
											value_type;

public:
	RWLock() : flag_(false) {
		container_ = container_ptr(new container_type);
	}
	virtual ~RWLock() {}

public:
	inline void putBack(const value_type& value) {
		spinlock_guard_type guard(lock_);
		container_->insert(container_->end(), value);
		flag_ = true;
	}

	inline void putFront(const value_type& value) {
		spinlock_guard_type guard(lock_);
		container_->insert(container_->begin(), value);
		flag_ = true;
	}

	inline bool get(container_ptr & container) {
		if (!flag_)
			return false;

		spinlock_guard_type guard(lock_);

		container = container_;
		container_ = container_ptr(new container_type);
		flag_ = false;

		return true;
	}

protected:
	container_ptr				container_;
	volatile bool				flag_;

private:
	ZC_TO_STRING("RWLock")

private:
	spinlock_type				lock_;
};

}}}

#endif /* RWLOCK_HPP_ */
