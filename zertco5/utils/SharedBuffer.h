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
#include <log/Log.h>

namespace zertcore { namespace utils { namespace mem {

typedef u8						byte;
typedef u32						size_type;

/**
 * Chunk
 */
struct Chunk
{
	size_type					size;
	byte						ptr[0];
};

typedef SMART_PTR(Chunk)					chunk_ptr;

}}}

namespace zertcore { namespace utils {

/**
 * SharedBuffer,
 *
 */
class SharedBuffer
{
public:
	typedef mem::byte						byte;
	typedef mem::size_type					size_type;

public:
	explicit SharedBuffer(const size_type& size) : offset_(0), size_(size) {
		void * m = malloc(size + sizeof(mem::Chunk));
		chunk_ = mem::chunk_ptr((mem::Chunk *)m);
		chunk_->size = size;
	}
	virtual ~SharedBuffer() {}

public:
	mem::byte& operator[] (const mem::size_type& offset) {
		if (!chunk_) {
			ZCLOG(FINAL) << "Chunk is NULL" << End;
		}

		mem::size_type index = offset_ + offset;
		if (index >= chunk_->size) {
			ZCLOG(FINAL) << "Size was out of memory(Offset:" << offset_ << " + Size:" << offset << ")" << End;
		}

		return chunk_->ptr[index];
	}
	const mem::byte& operator[] (const mem::size_type& offset) const {
		if (!chunk_) {
			ZCLOG(FINAL) << "Chunk is NULL" << End;
		}

		mem::size_type index = offset_ + offset;
		if (index >= chunk_->size) {
			ZCLOG(FINAL) << "Size was out of memory(Offset:" << offset_ << " + Size:" << offset << ")" << End;
		}

		return chunk_->ptr[index];
	}

public:
	mem::size_type size() const {return size_;}
	byte* data() {return &chunk_->ptr[offset_];}
	const byte* data() const {return &chunk_->ptr[offset_];}

public:
	SharedBuffer slice(const mem::size_type& offset) const {
		SharedBuffer buffer;
		buffer.chunk_ = chunk_;

		mem::size_type index = offset_ + offset;
		if (index >= chunk_->size) {
			ZCLOG(FINAL) << "Size was out of memory(Size:" << offset << ")" << End;
		}

		buffer.offset_ = index;
		buffer.size_ = chunk_->size - index;

		return buffer;
	}

	SharedBuffer slice(const mem::size_type& offset, const mem::size_type& length) const {
		SharedBuffer buffer;
		buffer.chunk_ = chunk_;

		mem::size_type index = offset_ + offset;
		if (index >= chunk_->size) {
			ZCLOG(FINAL) << "Size was out of memory(Size:" << offset << ")" << End;
		}

		buffer.offset_ = index;
		buffer.size_ = chunk_->size - index;

		if (buffer.size_ > length)
			buffer.size_ = length;

		return buffer;
	}

private:
	explicit SharedBuffer() : offset_(0), size_(0) {}

private:
	mem::size_type				offset_;
	mem::size_type				size_;

	mutable mem::chunk_ptr		chunk_;
};


class ReadOnlyBuffer;

} /* namespace concurrent */} /* namespace zertcore */

#endif /* BUFFER_H_ */
