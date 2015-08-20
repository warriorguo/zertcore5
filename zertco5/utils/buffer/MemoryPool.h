/*
 * MemoryPool.h
 *
 *  Created on: 2015年8月8日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_BUFFER_MEMORYPOOL_H_
#define ZERTCORE_UTILS_BUFFER_MEMORYPOOL_H_

#include <pch.h>
#include <utils/types.h>

#ifndef ZC_MEMORY_DEFAULT_BLOCK_SIZE
#  define ZC_MEMORY_DEFAULT_BLOCK_SIZE		128
#endif

namespace zertcore { namespace utils {

template <size_t UnitSize, size_t BlockSize = ZC_MEMORY_DEFAULT_BLOCK_SIZE>
class FixedMemoryPool
{
public:
	const static size_t						UNIT_SIZE = UnitSize;
	const static size_t						BLOCK_SIZE = BlockSize;

	struct BlockCell
	{
		byte					buffer[UNIT_SIZE];
	};

	typedef BlockCell						cell_type;
	typedef BlockCell*						cell_ptr;
	typedef vector<cell_ptr>				cell_list_type;

public:
	FixedMemoryPool() : cell_index_(0) { prepare(); }

public:
	void* allocate() {
		spinlock_guard_type guard(lock_);
		return fetchOne()->buffer;
	}
	void deallocate(void* mem) {
		spinlock_guard_type guard(lock_);
		addOne(mem);
	}

private:
	inline void addOne(void* mem) {
		ZC_ASSERT(cell_index_ < cell_list_.size());

		cell_ptr cell = cell_ptr((byte *)mem - (size_t)(&(((cell_ptr)0)->buffer)));
		cell_list_[cell_index_++] = cell;
	}
	inline cell_ptr fetchOne() {
		if (!cell_index_) {
			prepare();
		}

		ZC_DEBUG_ASSERT(cell_index_ > 0);
		ZC_DEBUG_ASSERT(cell_index_ < cell_list_.size());

		return cell_list_[cell_index_--];
	}

	inline void prepare() {
		cell_list_.resize(cell_list_.size() + BLOCK_SIZE);
		ZC_DEBUG_ASSERT(cell_index_ + BLOCK_SIZE < cell_list_.size());

		cell_ptr p = malloc(sizeof(cell_type) * BLOCK_SIZE);
		for (size_t i = 0; i < BLOCK_SIZE; ++i) {
			cell_list_[cell_index_++] = & p[i];
		}
	}

private:
	cell_list_type				cell_list_;
	size_t						cell_index_;

private:
	spinlock_type				lock_;
};

}}



#endif /* UTILS_BUFFER_DETAILS_MEMORYPOOL_H_ */
