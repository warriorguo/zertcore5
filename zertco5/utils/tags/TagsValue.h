/*
 * TagsValue.h
 *
 *  Created on: 2015-2-17
 *      Author: Administrator
 */

#ifndef ZERTCORE_TAGSVALUE_H_
#define ZERTCORE_TAGSVALUE_H_

#include <pch.h>
#include <utils/types.h>
#include <object/PoolObject.h>

#include "details/Tags.h"
#include "../group/Group.h"

namespace zertcore { namespace utils {

/**
 * TagsValue
 */
template <typename Value, typename ...Tn>
class TagsValue
{
public:
	typedef Value							value_type;

	typedef Group<value_type>				group_type;

public:
	typedef tags::Tags<NORMAL, Value, Tn...>
											tags_type;
public:
	typedef typename tags_type::ptr			node_ptr;

public:
	template <typename Tag>
	group_type find(const Tag& v) {
		typedef typename tags::TagTraits<NORMAL, node_ptr, Tag>::map_type map_type;
		typedef typename map_type::iterator iterator;
		pair<iterator, iterator> p;
		tags_.find(v, p);

		return p;
	}

	node_ptr make(const value_type& v) {
		node_ptr node = tags_type::create();
		node->value = v;
		return node;
	}
	void remove(node_ptr node) {
		tags_.erase(node);
	}

	template <typename Tag>
	void addTag(node_ptr node, const Tag& v) {
		tags_.insert(node, v);
	}

private:
	tags_type					tags_;
};

}}

#endif /* TAGSVALUE_H_ */
