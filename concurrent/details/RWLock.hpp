/*
 * RWLock.hpp
 *
 *  Created on: 2014Äê11ÔÂ6ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_RWLOCK_HPP_
#define ZERTCORE_RWLOCK_HPP_

#include <pch.h>

namespace zertcore{ namespace concurrent{ namespace details{

/**
 * RWLockContainer
 */
template <class STL_Container>
class RWLock
{
public:
	typedef STL_Container					container_type;
	typedef typename container_type::value_type
											value_type;

public:
	RWLock() : size_(0) {}

public:
	void put(const value_type& value) {
		mutex::scoped_lock lock(mu_);

		container_.insert(container_.end(), value);
		size_++;

		cond_.notify_one();
	}

	void put(const container_type& container) {
		mutex::scoped_lock lock(mu_);

		container_.insert(container_.end(),
				container.begin(), container_.end());
		size_ += container.size();

		cond_.notify_one();
	}

	bool get(container_type& container, bool block) {
		if (block) {
			mutex::scoped_lock lock(mu_);

			while(container_.empty())
				cond_.wait(lock);

			container = container_;
			container_.clear();

			size_ = 0;

			return !container.empty();
		}

		if (size_ > 0 && mu_.try_lock()) {
			container = container_;
			container_.clear();

			size_ = 0;
			mu_.unlock();

			return !container.empty();
		}

		return false;
	}

protected:
	container_type				container_;

private:
	atomic<size_t>				size_;

private:
	mutex						mu_;
	condition_variable			cond_;
};

}}}

#endif /* RWLOCK_HPP_ */
