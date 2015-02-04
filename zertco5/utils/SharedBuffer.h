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
 * SharedBuffer, to make it easy to share a buffer between threads
 *
 */
class SharedBuffer
{
public:
	typedef mem::byte						byte;
	typedef mem::size_type					size_type;

public:
	explicit SharedBuffer() : offset_(0), size_(0) {}
	explicit SharedBuffer(const size_type& size);
	virtual ~SharedBuffer() {}

public:
	bool resize(const size_type& size);
	SharedBuffer& getOwned();

public:
	mem::byte& operator[] (const mem::size_type& offset);
	const mem::byte& operator[] (const mem::size_type& offset) const;

public:
	/**
	 * copy on write
	 */
	bool assign(const void* d, const size_type& size);
	bool assign(const string& str);
	bool write(const void* d, const size_type& size) {
		return assign(d, size);
	};

public:
	mem::size_type size() const {return size_;}
	bool empty() const {return size_ == 0;}

	byte* data() {return &chunk_->ptr[offset_];}
	const byte* data() const {return &chunk_->ptr[offset_];}

public:
	SharedBuffer slice(const mem::size_type& offset) const;
	SharedBuffer slice(const mem::size_type& offset, const mem::size_type& length) const;

private:
	bool realloc(const size_type& size);

private:
	size_type					offset_;
	size_type					size_;

	mutable mem::chunk_ptr		chunk_;
};


class ReadOnlyBuffer;

} /* namespace concurrent */} /* namespace zertcore */

#endif /* BUFFER_H_ */
