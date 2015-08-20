/*
 * QuadTree.h
 *
 *  Created on: 2015年6月24日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_COLLISION_QUADTREE_H_
#define ZERTCORE_UTILS_COLLISION_QUADTREE_H_

#include <pch.h>
#include <utils/types.h>

#include <object/PoolObject.h>

namespace zertcore { namespace utils {
using namespace zertcore::object;

namespace details {

enum dir_type {
	DIR_NONE								= 0,
	DIR_TL									= 1,
	DIR_TR									= 2,
	DIR_BL									= 3,
	DIR_BR									= 4,
};

}

}}

namespace zertcore { namespace utils {

template <typename T>
class QuadTreeNode
{
public:
	typedef PoolObject<T>					data_type;
	typedef typename data_type::ptr			data_ptr;

public:
	bool isDataNode() const;

private:
	data_ptr				data_;
	u32						data_amount_{0};

	details::dir_type		dir_{details::DIR_NONE};
};


template <typename T>
class QuadTree
{
public:

private:
};

}}


#endif /* UTILS_COLLISION_QUADTREE_H_ */
