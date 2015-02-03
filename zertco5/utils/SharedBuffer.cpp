/*
 * SharedBuffer.cpp
 *
 *  Created on: 2015年1月31日
 *      Author: Administrator
 */
#include "SharedBuffer.h"

namespace zertcore { namespace utils {

SharedBuffer::SharedBuffer(const size_type& size) : offset_(0), size_(0) {
	realloc(size);
}

bool SharedBuffer::resize(const size_type& size) {
	if (size > size_) {
		realloc(size);
	}
	return true;
}

SharedBuffer& SharedBuffer::getOwned() {
	if (size_ > 0)
		realloc(size_);

	return *this;
}

mem::byte& SharedBuffer::operator[] (const mem::size_type& offset) {
	if (!chunk_) {
		ZCLOG(FINAL) << "Chunk is NULL" << End;
	}

	mem::size_type index = offset_ + offset;
	if (index >= chunk_->size) {
		ZCLOG(FINAL) << "Size was out of memory(Offset:" << offset_ << " + Size:" << offset << ")" << End;
	}

	return chunk_->ptr[index];
}
const mem::byte& SharedBuffer::operator[] (const mem::size_type& offset) const {
	return operator[] (offset);
}

bool SharedBuffer::assign(const void* d, const size_type& size) {
	resize(size);

	if (!d || size > chunk_->size)
		return false;

	memcpy(chunk_->ptr, d, size);
	return true;
}

bool SharedBuffer::assign(const string& str) {
	return assign(str.data(), str.size());
}

SharedBuffer SharedBuffer::slice(const mem::size_type& offset) const {
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

SharedBuffer SharedBuffer::slice(const mem::size_type& offset, const mem::size_type& length) const {
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

bool SharedBuffer::realloc(const size_type& size) {
	void * m = malloc(size + sizeof(mem::Chunk));
	if (size_ > 0) {
		memcpy(m, chunk_->ptr, chunk_->size);
	}

	chunk_ = mem::chunk_ptr((mem::Chunk *)m);
	chunk_->size = size;

	return true;
}

}}
