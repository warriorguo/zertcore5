/*
 * PoolAllocator.h
 *
 *  Created on: 2015年6月19日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_BUFFER_MEMORYALLOCATOR_H_
#define ZERTCORE_UTILS_BUFFER_MEMORYALLOCATOR_H_

#include <pch.h>
#include <utils/types.h>

#ifndef ZC_MEMORY_DEFAULT_BLOCK_SIZE
#  define ZC_MEMORY_DEFAULT_BLOCK_SIZE		128
#endif

namespace zertcore { namespace utils {

/**
 * Notice: this was NOT recommend for using right now,
 * in Testing, when BlockSize bigger enough, it just faster a tiny little than pure malloc
 * and when the BlockSize was small, it did far way slower than malloc!
 *
 * MemoryAllocator<UnitSize, BlockSize>
 */
template <size_t UnitSize, size_t BlockSize = ZC_MEMORY_DEFAULT_BLOCK_SIZE>
class MemoryAllocator
{
public:
	const static size_t						unit_size = UnitSize;
	const static size_t						suggest_block_size = BlockSize;

public:
	struct Block;
	typedef list<Block*>					block_list_type;

	/**
	 * Cell
	 */
	struct Cell
	{
		Block*					block;
		byte					addr[unit_size];
	};
	/**
	 * Block
	 */
	struct Block
	{
		u32						amount;
		typename block_list_type::iterator
								iter;
		Cell*					cells_ptr[suggest_block_size];
		Cell					cells[suggest_block_size];
	};

public:
	MemoryAllocator() {
		ZC_DEBUG_ASSERT(suggest_block_size > 0);
	}

	void init() {prepare();}

public:
	void* allocate() {
		spinlock_guard_type guard(lock_);
		return fetch();
	}
	void deallocate(void* mem) {
		ZC_ASSERT(mem);
		spinlock_guard_type guard(lock_);
		dispose(mem);
	}

private:
	bool prepare() {
		Block* block = (Block *)malloc(sizeof(Block));
		if (!block)
			return false;

		block->amount = suggest_block_size;
		for (size_t i = 0; i < suggest_block_size; ++i) {
			block->cells_ptr[i] = &block->cells[i];
			block->cells[i].block = block;
		}

		block->iter = block_list_.insert(block_list_.begin(), block);
		return true;
	}
	void* fetch() {
		ZC_DEBUG_ASSERT(!block_list_.empty());

		Block* block = *block_list_.begin();
		if (block->amount == 0) {
			if (!prepare()) return nullptr;
		}

		block = *block_list_.begin();
		Cell* cell = block->cells_ptr[--block->amount];

		return (void *)cell->addr;
	}
	void dispose(void* mem) {
		Cell* cell = (Cell *)((byte *)mem - (size_t)(&(((Cell *)0)->addr)));
		Block* block = cell->block;

		ZC_ASSERT(block->amount < suggest_block_size);
		ZC_ASSERT(!block_list_.empty());

		block->cells_ptr[block->amount++] = cell;

		if (block->amount == suggest_block_size &&
				/**
				 * if all mem returned just stay 1 block in the list
				 */
				block_list_.size() > 1) {
			block_list_.erase(block->iter);
			free(block);
		}
		else {
			Block* current_block = *block_list_.begin();
			/**
			 * if the head of list free amount was less than this one
			 * swap them
			 */
			if (block->amount > current_block->amount) {
				*(block->iter) = current_block;
				*(current_block->iter) = block;

				typename block_list_type::iterator it = block->iter;
				block->iter = current_block->iter;
				current_block->iter = it;
			}
		}
	}

private:
	static spinlock_type		lock_;
	static size_t				mem_index_;
	static block_list_type		block_list_;
};

template <size_t T, size_t Size>
spinlock_type					MemoryAllocator<T, Size>::lock_;
template <size_t T, size_t Size>
size_t							MemoryAllocator<T, Size>::mem_index_ = 0;
template <size_t T, size_t Size>
typename MemoryAllocator<T, Size>::block_list_type
								MemoryAllocator<T, Size>::block_list_;

}}


#endif /* UTILS_BUFFER_POOLALLOCATOR_H_ */
