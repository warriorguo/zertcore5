/*
 * msgpack_stream_intend.cpp
 *
 *  Created on: 2015Äê1ÔÂ23ÈÕ
 *      Author: Administrator
 */

#include "msgpack_stream_intend.h"

namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {

void zc_safe_input_object::realloc(bool dump) {
	if (!via.str.size) {
		return;
	}

	uint32_t size = via.str.size;

	switch (type) {
	case type::STR:
		break;
	case type::BIN:
		break;
	case type::ARRAY:
		size *= sizeof(object);
		break;
	case type::MAP:
		size *= sizeof(object_kv);
		break;
	case type::EXT:
		size++;
		break ;

	default:
		return ;
	}

	char* mem = new char[size];
	ptr = smart_ptr(mem);

	if (dump) {
		::memcpy(mem, via.bin.ptr, via.bin.size);
	}
	via.str.ptr = mem;
}

}}

