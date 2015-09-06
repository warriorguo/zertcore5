/*
 * UpdateList.h
 *
 *  Created on: 2015年7月26日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_UPDATELIST_UPDATELIST_H_
#define ZERTCORE_UTILS_UPDATELIST_UPDATELIST_H_

#include <pch.h>
#include <utils/types.h>

namespace zertcore { namespace utils {

namespace details {

template <typename Container, typename T>
inline void add(Container& container, const T& value);

template <typename T, typename C>
inline void add(list<T, C>& container, const T& value) {
	container.push_back(value);
}

template <typename T, typename C, typename A>
inline void add(set<T, C, A>& container, const T& value) {
	container.insert(value);
}

template <typename T, typename H, typename C, typename A>
inline void add(unordered_set<T, H, C, A>& container, const T& value) {
	container.insert(value);
}

template <typename T, typename C, typename A>
inline void add(multiset<T, C, A>& container, const T& value) {
	container.insert(value);
}

template <typename T, typename H, typename C, typename A>
inline void add(unordered_multiset<T, H, C, A>& container, const T& value) {
	container.insert(value);
}

}

/**
 * UpdateList
 */
template <typename STL_Container>
class UpdateList
{
public:
	typedef STL_Container					container_type;
	typedef container_type*					container_ptr;
	typedef typename STL_Container::value_type
											value_type;

public:
	UpdateList() : container_(nullptr) {}
	~UpdateList() {
		if (container_) {
			delete container_;
		}
	}

public:
	std::size_t size() const {
		if (container_) return container_->size();
		return 0;
	}
	bool add(const value_type& value) {
		spinlock_guard_type guard(lock_);

		if (!container_)
			container_ = new container_type;

		details::add(*container_, value);
		return true;
	}

	template <typename Handler>
	void foreach(Handler handler, bool clearup = true) {
		container_ptr container(nullptr);

		do {
			spinlock_guard_type guard(lock_);
			container = container_;

			if (clearup)
				container_ = nullptr;
		}
		while(false);

		if (!container) {
			return ;
		}

		for (auto it = container->begin(); it != container->end(); ++it) {
			handler(*it);
		}
		delete container;
	}

private:
	void clearUp() {
		if (container_) {
			container_ = nullptr;
		}
	}


private:
	container_ptr				container_;
	spinlock_type				lock_;
};

}}


#endif /* UTILS_UPDATELIST_UPDATELIST_H_ */
