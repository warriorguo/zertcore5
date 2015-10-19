/*
 * Group.h
 *
 *  Created on: 2015-2-20
 *      Author: Administrator
 */

#ifndef ZERTCORE_GROUP_H_
#define ZERTCORE_GROUP_H_

#include <pch.h>
#include <object/PoolObject.h>

namespace zertcore { namespace utils {
using namespace zertcore::object;

/**
 * GroupImpl<Value>
 */
template <typename Value>
class GroupImpl :
		public PoolObject<GroupImpl<Value> >
{
public:
	typedef Value							value_type;
	typedef vector<value_type>				value_list_type;
	typedef typename value_list_type::iterator
											iterator;
	typedef typename value_list_type::const_iterator
											const_iterator;
public:
	GroupImpl() {}

	template <typename Iterator>
	GroupImpl(const Iterator& begin, const Iterator& end) {
		for_each(begin, end, [&] (const value_type& v) {
			list_.push_back(v);
		});
	}

public:
	virtual ~GroupImpl() {}

public:
	iterator begin() {return list_.begin();}
	iterator end() {return list_.end();}

	const_iterator begin() const {return list_.begin();}
	const_iterator end() const {return list_.end();}

public:
	value_type& at(const u32& index) {
		ZC_ASSERT(index < size());
		return list_.at(index);
	}
	const value_type& at(const u32& index) const {
		ZC_ASSERT(index < size());
		return list_.at(index);
	}

public:
	u32 size() const {return list_.size();}
	bool empty() const {return list_.empty();}

public:
	template <typename Handler>
	void foreach(const Handler& handler) {
		for (auto it = list_.begin(); it != list_.end(); ++it) {
			handler(*it);
		}
	}
	template <typename ComplareHandler>
	void sort(const ComplareHandler& handler) {
		std::sort(list_.begin(), list_.end(), handler);
	}

public:
	void insert(const value_type& v) {
		list_.push_back(v);
	}
	bool erase(iterator it) {
		list_.erase(it);
		return true;
	}

private:
	value_list_type				list_;

	ZC_TO_STRING(
		"size" << list_.size()
	);
};

/**
 * Group<Value>
 */
template <typename Value>
class Group
{
public:
	typedef GroupImpl<Value>				group_impl_type;
	typedef typename group_impl_type::ptr	group_impl_ptr;

	typedef typename group_impl_type::iterator
											iterator;
	typedef typename group_impl_type::const_iterator
											const_iterator;

public:
	template <typename Iterator>
	Group(const Iterator& begin, const Iterator& end) {
		group_ = group_impl_type::create(begin, end);
	}
	template <typename Iterator>
	Group(pair<Iterator, Iterator>& p) {
		group_ = group_impl_type::create(p.first, p.second);
	}
	Group(const Group& g) : group_(g.group_) {}
	Group() {
		group_ = group_impl_type::create();
	}

public:
	const Value& at(const u32& index) const {
		return group_->at(index);
	}
	Value& at(const u32& index) {
		return group_->at(index);
	}

public:
	u32 size() const {return group_->size();}
	bool empty() const {return group_->empty();}

public:
	iterator begin() {return group_->begin();}
	iterator end() {return group_->end();}

	const_iterator begin() const {return group_->begin();}
	const_iterator end() const {return group_->end();}


public:
	void add(const Value& v) {
		group_->insert(v);
	}
	bool erase(iterator it) {
		return group_->erase(it);
	}

public:
	template <typename Handler>
	void foreach(const Handler& handler) {
		group_->foreach(handler);
	}
	template <typename ComplareHandler>
	void sort(const ComplareHandler& handler) {
		group_->sort(handler);
	}

private:
	group_impl_ptr				group_;
};

}}

#endif /* GROUP_H_ */
