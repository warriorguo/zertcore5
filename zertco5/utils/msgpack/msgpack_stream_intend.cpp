/*
 * msgpack_stream_intend.cpp
 *
 *  Created on: 2015��1��23��
 *      Author: Administrator
 */

#include "msgpack_stream_intend.h"

namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {

void zc_safe_input_object::alloc() {
	if (!size)
		return ;

	ptr = smart_ptr(new char[real_size(size)]);
}

void zc_safe_input_object::dump() {
	if (!via.str.size) {
		via.str.ptr = ptr.get();
		return;
	}

	if (via.str.size > size) {
		size = via.str.size;
		alloc();
	}

	char* mem = ptr.get();
	if (mem == via.str.ptr)
		return ;

	memcpy(mem, via.str.ptr, real_size(via.str.size));
	via.str.ptr = mem;
}

uint32_t zc_safe_input_object::real_size(const uint32_t& s) const {
	uint32_t need_size = s;

	switch (type) {
	case type::STR:
		break;
	case type::BIN:
		break;
	case type::ARRAY:
		need_size *= sizeof(object);
		break;
	case type::MAP:
		need_size *= sizeof(object_kv);
		break;
	case type::EXT:
		need_size++;
		break ;

	default:
		return 0;
	}

	return need_size;
}

}}

