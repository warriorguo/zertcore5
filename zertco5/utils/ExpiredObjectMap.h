/*
 * ExpiredObjectMap.h
 *
 *  Created on: 2015年4月23日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_EXPIREDOBJECTMAP_H_
#define ZERTCORE_UTILS_EXPIREDOBJECTMAP_H_

#include <pch.h>

#include <utils/types.h>
#include <utils/time/Timer.h>
#include <thread/ThreadHandler.h>

#include <utils/tools/SameThreadChecker.h>

namespace zertcore { namespace utils {
using namespace zertcore::concurrent;
}}

namespace zertcore { namespace utils {

/**
 * ExpiredObjectMap
 */
template <typename Key, typename Object>
class ExpiredObjectMap
{
	typedef ExpiredObjectMap<Key, Object>	self;

public:
	typedef Key								key_type;
	typedef Object							value_type;
	typedef Object*							value_ptr;

	typedef timer_type::counter_type		counter_type;
	typedef multimap<counter_type, key_type>
											expired_map_type;

public:
	typedef function<void (const key_type&)>
											expired_handler_type;
public:
	struct Cell
	{
		value_type				value;
		typename expired_map_type::iterator
								link;
	};
	typedef unordered_map<key_type, Cell>	map_type;

public:
	value_ptr get(const key_type& key) {
		spinlock_guard_type guard(lock_);

		auto it = data_.find(key);
		if (it == data_.end())
			return nullptr;

		return &(it->second.value);
	}

	const value_ptr get(const key_type& key) const {
		spinlock_guard_type guard(lock_);

		auto it = data_.find(key);
		if (it == data_.end())
			return nullptr;

		return &(it->second.value);
	}

	tick_type getExpiredTick(const key_type& key) const {
		spinlock_guard_type guard(lock_);

		auto it = data_.find(key);
		if (it == data_.end())
			return 0;

		counter_type now(Now);
		return it->second.link->first - now;
	}

	bool set(const key_type& key, const value_type& value, const tick_type& time_out) {
		remove(key);
		do {
		spinlock_guard_type guard(lock_);

		counter_type now(Now);

		Cell cell;
		cell.value = value;
		cell.link = expired_.insert(typename expired_map_type::value_type(now + time_out, key));
		data_.insert(typename map_type::value_type(key, cell));

		}while(false);

		return true;
	}

	bool remove(const key_type& key) {
		spinlock_guard_type guard(lock_);

		auto it = data_.find(key);
		if (it == data_.end())
			return false;

		expired_.erase(it->second.link);
		data_.erase(it);
		return true;
	}

	bool refresh(const key_type& key, const tick_type& time_out) {
		spinlock_guard_type guard(lock_);

		auto it = data_.find(key);
		if (it == data_.end())
			return false;

		counter_type now(Now);

		expired_.erase(it->second.link);
		it->second.link = expired_.insert(typename expired_map_type::value_type(now + time_out, key));

		return true;
	}

private:
	void update(const tick_type& intval) {
		spinlock_guard_type guard(lock_);
		thread_checker_.check();

		counter_type now(Now);
		timer_.reset();

		for (auto it = expired_.begin(); it != expired_.end(); ) {
			auto hit = it; ++it;

			if (now < hit->first) {
				break;
			}

			const key_type& key = hit->second;

			if (expired_handler_)
				expired_handler_(key);

			data_.erase(key);
			expired_.erase(hit);
		}
	}

public:
	void init(expired_handler_type handler, const counter_type& intval = 1000) {
		if (timer_.isWorking())
			return ;

		thread_checker_.init();
		expired_handler_ = handler;

		timer_.onExpired(bind(&self::update, this, _1));
		timer_.start(intval);
	}

private:
	map_type					data_;
	expired_map_type			expired_;

private:
	expired_handler_type		expired_handler_;
	timer_type					timer_;

private:
	mutable spinlock_type		lock_;
	SameThreadChecker			thread_checker_;
};

}}


#endif /* UTILS_EXPIREDOBJECTMAP_H_ */
