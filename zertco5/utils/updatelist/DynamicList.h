/*
 * DynamicList.h
 *
 *  Created on: 2015年8月7日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_UPDATELIST_DYNAMICLIST_H_
#define ZERTCORE_UTILS_UPDATELIST_DYNAMICLIST_H_

#include <pch.h>
#include <utils/types.h>

namespace zertcore { namespace utils {

template <class Value, std::size_t Size>
class DynamicList
{
	const static std::size_t				CACHE_SIZE = 3;
public:
	typedef Value							value_type;
	typedef array<value_type, Size>			container_type;
	typedef container_type*					container_ptr;

	typedef array<container_ptr, 3>			cache_type;

public:
	DynamicList() : head_list_(nullptr), tail_list_(nullptr), head_index_(0), tail_index_(0) {
		for (std::size_t i = 0; i < CACHE_SIZE; ++i) {
			head_cache_log_[i] = head_cache_[i] = nullptr;
		}
		for (std::size_t i = 0; i < CACHE_SIZE; ++i) {
			tail_cache_log_[i] = tail_cache_[i] = nullptr;
		}
		cache_head_index_ = cache_tail_index_ = 0;
	}

	~DynamicList() {
		for (std::size_t i = 0; i < CACHE_SIZE; ++i) {
			if (head_cache_log_[i])
				delete head_cache_log_[i];
		}
		for (std::size_t i = 0; i < CACHE_SIZE; ++i) {
			if (tail_cache_log_[i])
				delete tail_cache_log_[i];
		}

	}

public:
	void prepareHead() {
		for (std::size_t i = 0; i < CACHE_SIZE; ++i) {
			head_cache_log_[i] = head_cache_[i] = new container_type;
		}
		cache_head_index_ = CACHE_SIZE;
		fetchHead();
	}
	void prepareTail() {
		for (std::size_t i = 0; i < CACHE_SIZE; ++i) {
			tail_cache_log_[i] = tail_cache_[i] = new container_type;
		}
		cache_tail_index_ = CACHE_SIZE;
		fetchTail();
	}

public:
	bool addHead(const value_type& v) {
		spinlock_guard_type guard(head_lock_);
//		ZC_ASSERT(head_list_);
		if (head_index_ >= Size) {
			return false;
		}

		(*head_list_)[head_index_++] = v;
		return true;

	}
	bool addTail(const value_type& v) {
		spinlock_guard_type guard(tail_lock_);
//		ZC_ASSERT(tail_list_);
		if (tail_index_ >= Size) {
			return false;
		}

		(*tail_list_)[tail_index_++] = v;
		return true;
	}

public:
	container_ptr getHead(std::size_t& size) {
		spinlock_guard_type guard(head_lock_);
		container_ptr list = head_list_;
		size = head_index_;

		if (!head_index_) {
			return nullptr;
		}

		fetchHead();
		return list;
	}
	container_ptr getTail(std::size_t& size) {
		spinlock_guard_type guard(tail_lock_);
		container_ptr list = tail_list_;
		size = tail_index_;

		if (!tail_index_) {
			return nullptr;
		}

		fetchTail();
		return list;
	}

private:
	void fetchHead() {
		ZC_DEBUG_ASSERT(cache_head_index_ > 0);
		head_list_ = head_cache_[--cache_head_index_];
		head_index_ = 0;
	}
	void fetchTail() {
		ZC_DEBUG_ASSERT(cache_tail_index_ > 0);
		tail_list_ = tail_cache_[--cache_tail_index_];
		tail_index_ = 0;
	}

public:
	void releaseHead(container_ptr p) {
		ZC_DEBUG_ASSERT(cache_head_index_ < CACHE_SIZE);
		head_cache_[cache_head_index_++] = p;
	}
	void releaseTail(container_ptr p) {
		ZC_DEBUG_ASSERT(cache_tail_index_ < CACHE_SIZE);
		tail_cache_[cache_tail_index_++] = p;
	}

private:
	container_ptr				head_list_,
								tail_list_;
	std::size_t					head_index_,
								tail_index_;
	spinlock_type				head_lock_,
								tail_lock_;
	cache_type					head_cache_, head_cache_log_,
								tail_cache_, tail_cache_log_;
	std::size_t					cache_head_index_,
								cache_tail_index_;
};

}}


#endif /* UTILS_UPDATELIST_DYNAMICLIST_H_ */
