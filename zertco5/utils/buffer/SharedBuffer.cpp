/*
 * SharedBuffer.cpp
 *
 *  Created on: 2015年1月31日
 *      Author: Administrator
 */
#include "SharedBuffer.h"
#include <log/Log.h>

namespace zertcore { namespace utils {

SharedBuffer::SharedBuffer(const size_type& size) :
		offset_(0), size_(0), owned_(false) {
	realloc(size);
}

bool SharedBuffer::
reserve(const size_type& size) {
	if (!owned_ || !chunk_ || size > chunk_->size) {
		if (!realloc(size))
			return false;
	}

	return true;
}

bool SharedBuffer::
resize(const size_type& size) {
	if (!reserve(size))
		return false;
	return true;
}

SharedBuffer& SharedBuffer::
getOwned() {
	if (owned_)
		return *this;

	if (size_ > 0)
		realloc(size_);

	return *this;
}

mem::byte& SharedBuffer::
operator[] (const mem::size_type& offset) {
	if (!chunk_) {
		ZCLOG(FATAL) << "Chunk is NULL" << End;
	}

	mem::size_type index = offset_ + offset;
	if (index >= chunk_->size) {
		ZCLOG(FATAL) << "Size was out of memory(Offset:"
				<< offset_ << " + Size:" << offset << ")" << End;
	}

	return chunk_->ptr[index];
}

const mem::byte& SharedBuffer::
operator[] (const mem::size_type& offset) const {
	if (!chunk_) {
		ZCLOG(FATAL) << "Chunk is NULL" << End;
	}

	mem::size_type index = offset_ + offset;
	if (index >= chunk_->size) {
		ZCLOG(FATAL) << "Size was out of memory(Offset:"
				<< offset_ << " + Size:" << offset << ")" << End;
	}

	return chunk_->ptr[index];
}

SharedBuffer& SharedBuffer::
operator = (const SharedBuffer& buffer) {
	chunk_ = buffer.chunk_;
	offset_ = buffer.offset_;
	size_ = buffer.size_;
	owned_ = false;

	return *this;
}

bool SharedBuffer::
operator == (const SharedBuffer& buffer) const {
	u32 s = size();
	if (s != buffer.size())
		return false;

	const byte* d = data();
	const byte* ld = buffer.data();
	ZC_DEBUG_ASSERT(d);


	for (u32 i = 0; i < s; ++i) {
		if (d[i] != ld[i])
			return false;
	}

	return true;
}

bool SharedBuffer::
add(const void* d, const size_type& size) {
	if (!d) return false;
	if (!size) return false;

	size_type offset = size_;
	size_type total_size = offset + size;

	if (!chunk_ || size > capacity() || !owned_) {
		if (!realloc(total_size))
			return false;
	}

	size_ = total_size;
	memcpy(data() + offset, d, size);
	return true;
}

bool SharedBuffer::
add(const string& str) {
	return add(str.data(), str.size());
}

bool SharedBuffer::
add(const SharedBuffer& buffer) {
	return add(buffer.data(), buffer.size());
}

bool SharedBuffer::
assign(const void* d, const size_type& size) {
	if (!d) return false;

	size_ = 0;
	if (!owned_ || !chunk_ || size > chunk_->size) {
		if (!realloc(size))
			return false;
	}

	ZC_ASSERT(owned_);

	if (size > chunk_->size)
		return false;

	memcpy(chunk_->ptr, d, size);
	size_ = size;

	return true;
}

void SharedBuffer::
print() const {
	if (!chunk_) {
		::printf("EMPTY\n");
		return ;
	}

	for (size_t i = 0; i < size(); ++i) {
		mem::byte b = operator[](i);
		if ((i % 16) == 0) {
			::printf("\n");
		}
		::printf("%x ", b);
	}
	::printf("\n");
}

bool SharedBuffer::
assign(const string& str) {
	return assign(str.data(), str.size());
}

void SharedBuffer::
clear() {
	size_ = 0;
}

SharedBuffer SharedBuffer::
slice(const mem::size_type& offset) const {
	SharedBuffer buffer;
	buffer.chunk_ = chunk_;

	mem::size_type index = offset_ + offset;
	if (index >= chunk_->size) {
		ZCLOG(FATAL) << "Size was out of memory(Size:" << offset << ")" << End;
	}

	buffer.offset_ = index;
	buffer.size_ = size_ - offset;

	return buffer;
}

SharedBuffer SharedBuffer::
slice(const mem::size_type& offset, const mem::size_type& length) const {
	SharedBuffer buffer;
	buffer.chunk_ = chunk_;

	mem::size_type index = offset_ + offset;
	if (index >= chunk_->size) {
		ZCLOG(FATAL) << "Size was out of memory(Size:" << offset << ")" << End;
	}

	buffer.offset_ = index;

	if (length > chunk_->size - index)
		ZCLOG(FATAL) << "Length:" << length << " > chunk_->size(" << chunk_->size << ") - index("
			<< index << ") offset_=" << offset_ << " offset=" << offset << End;

	buffer.size_ = length;

	return buffer;
}

bool SharedBuffer::
erase(const mem::size_type& offset, mem::size_type length) {
	ZC_ASSERT(chunk_);

	if (offset > size_)
		return false;

	if (length == 0)
		return true;

	if (length > size_ - offset) {
		size_ = offset;
		return true;
	}

//	ZCLOG(NOTE) << "memcpy offset=" << offset << " length=" << length << " size=" << size_ << End;
	memcpy(data() + offset, data() + offset + length, size_ - offset - length);
	size_ -= length;

	return true;
}

bool SharedBuffer::
realloc(const size_type& size) {
	ZC_ASSERT(size_ <= size);
//	ZCLOG(NOTE) << "realloc to " << size << End;

	void * m = malloc(size + sizeof(mem::Chunk));
	if (chunk_) {
		memcpy(&((mem::Chunk *)m)->ptr[0], &chunk_->ptr[offset_], size_);
	}

	chunk_ = mem::chunk_ptr((mem::Chunk *)m);
	chunk_->size = size;

	//revision the offset
	offset_ = 0;
	owned_ = true;

	return true;
}

namespace mem {

void Chunk::operator delete(void *ptr) {
	free(ptr);
}

}

}}

