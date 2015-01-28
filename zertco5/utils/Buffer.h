/*
 * Buffer.h
 *
 *  Created on: 2015��1��8��
 *      Author: Administrator
 */

#ifndef ZERTCORE_BUFFER_H_
#define ZERTCORE_BUFFER_H_

#include <pch.h>
#include <utils/types.h>

namespace zertcore { namespace utils {
using namespace zertcore::object;
}}

namespace zertcore { namespace utils { namespace mem {

typedef u8						byte;
typedef u32						size_type;

/**
 * Chunk
 */
struct Chunk
{
	byte*						ptr;
	size_type					size;

	explicit Chunk(u32 sz) : ptr(null), size(sz) {ptr = malloc(size);}
	explicit Chunk(Chunk& chunk) : ptr(chunk.ptr), size(chunk.size) {}
	~Chunk() {
		if (ptr) free(ptr);
		ptr = null;
		size = 0;
	}
};

typedef SMART_PTR(Chunk)					chunk_ptr;

}}}

namespace zertcore { namespace utils {

/**
 * Buffer
 */
class Buffer
{
public:
	Buffer(const mem::size_type& size);
	virtual ~Buffer();

public:
	mem::byte& operator[] (const mem::size_type& size) {
		if (!chunk_) {
			LOG(FINAL) << "Chunk is NULL" << End;
		}

		mem::size_type index = offset_ + size;
		if (index >= chunk_->size) {
			LOG(FINAL) << "Size was out of memory(Offset:" << offset_ << " + Size:" << size << ")" << End;
		}

		return chunk_->ptr[index];
	}
	const mem::byte& operator[] (const mem::size_type& size) const {
		if (!chunk_) {
			LOG(FINAL) << "Chunk is NULL" << End;
		}

		mem::size_type index = offset_ + size;
		if (index >= chunk_->size) {
			LOG(FINAL) << "Size was out of memory(Offset:" << offset_ << " + Size:" << size << ")" << End;
		}

		return chunk_->ptr[index];
	}

public:
	mem::size_type size() const {return size_ - offset_;}

public:
	Buffer offset(const mem::size_type& size) const {
		Buffer buffer;
		buffer.chunk_ = chunk_;

		mem::size_type index = offset_ + size;
		if (index >= chunk_->size) {
			LOG(FINAL) << "Size was out of memory(Size:" << size << ")" << End;
		}

		buffer.offset_ = index;
		buffer.size_ = chunk_->size - index;

		return buffer;
	}

private:
	Buffer() : offset_(0), size_(0) {}

private:
	mem::size_type				offset_;
	mem::size_type				size_;

	mutable mem::chunk_ptr		chunk_;
};


class ReadOnlyBuffer;

} /* namespace concurrent */} /* namespace zertcore */

#endif /* BUFFER_H_ */
