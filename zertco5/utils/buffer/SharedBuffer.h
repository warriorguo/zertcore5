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

	void operator delete(void *ptr);
};

typedef SMART_PTR(Chunk)					chunk_ptr;

}}}

namespace zertcore { namespace utils {

/**
 * SharedBuffer, to make it easy to share a buffer between threads
 * And follow the COW rule
 *
 */
/**
 * (15.5.25 update) add serialization for SharedBuffer, but!
 * IMPORTANT: just simply handle it as string, so the BINARY data would be a big problem!
 * next consider use base64 to handle it, (but performance would be low)
 *
 * (15.6.10 update) SharedBuffer just support for BSON format since it could handle BIN data.
 */
class SharedBuffer
{
public:
	enum {
		MAX_SIZE							= 1024 * 1024 * 24,
	};

public:
	typedef mem::byte						byte;
	typedef mem::size_type					size_type;

public:
	explicit SharedBuffer() : offset_(0), size_(0), owned_(false) {}
	explicit SharedBuffer(const size_type& size);
	virtual ~SharedBuffer() {}

public:
	bool reserve(const size_type& size);
	bool resize(const size_type& size);

	SharedBuffer& getOwned();

public:
	mem::byte& operator[] (const mem::size_type& offset);
	const mem::byte& operator[] (const mem::size_type& offset) const;

	SharedBuffer& operator = (const SharedBuffer& buffer);
	bool operator == (const SharedBuffer& buffer) const;
	bool operator != (const SharedBuffer& buffer) const { return !(*this == buffer);}

public:
	/**
	 * copy on write
	 */
	bool add(const void* d, const size_type& size);
	bool add(const string& str);
	bool add(const SharedBuffer& buffer);

	bool assign(const void* d, const size_type& size);
	bool assign(const string& str);

	bool write(const void* d, const size_type& size) {
		return add(d, size);
	};
	void clear();
	void destroy();

public:
	void print() const;

public:
	mem::size_type size() const {return size_;}
	bool setSize(const mem::size_type& size) {
		ZC_ASSERT(chunk_);

		if (size > chunk_->size - offset_)
			return false;

		size_ = size;
		return true;
	}

	mem::size_type capacity() const {
		if (!chunk_)
			return 0;
		return chunk_->size - offset_ - size_;
	}
	bool empty() const {return size_ == 0;}

	byte* data() {
		if (!chunk_) return nullptr;
		return &chunk_->ptr[offset_];
	}
	const byte* data() const {
		if (!chunk_) return nullptr;
		return &chunk_->ptr[offset_];
	}

public:
	SharedBuffer slice(const mem::size_type& offset) const;
	SharedBuffer slice(const mem::size_type& offset, const mem::size_type& length) const;

	bool erase(const mem::size_type& offset, mem::size_type length);

public:
	bool writable() const {
		return !!chunk_;
	}

private:
	bool realloc(const size_type& size);

private:
	size_type					offset_;
	size_type					size_;

	bool						owned_;
	mutable mem::chunk_ptr		chunk_;
};


} /* namespace utils */} /* namespace zertcore */

namespace zertcore {

/**
 * its not safe when assign a shared buffer to a thread handler,
 * mainly because its content may changed against when the thread handler push action took
 */
template<>
struct AssignTraits<utils::SharedBuffer> : UNIQUE {};

template <>
inline utils::SharedBuffer assign(utils::SharedBuffer& b, const UNIQUE& _) {
	return b.getOwned();
}
template <>
inline utils::SharedBuffer assign(const utils::SharedBuffer& b, const UNIQUE& _) {
	utils::SharedBuffer t(b);
	return t.getOwned();
}

}

#endif /* BUFFER_H_ */
