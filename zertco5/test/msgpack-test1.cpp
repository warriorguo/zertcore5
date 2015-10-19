/*
 * msgpack-test1.cpp
 *
 *  Created on: 2015Äê1ÔÂ22ÈÕ
 *      Author: Administrator
 */


#include <utils/msgpack/msgpack_stream_intend.h>


int main() {
	msgpack::input_object_map_type pk;
	pk["show"] = 1;
	pk["me"] = "haha";
/**
	msgpack::object wobj;
	msgpack::sbuffer sbuf;
	msgpack::pack(wobj, pk);

	msgpack::unpacked unpack;
	msgpack::unpack(&unpack, sbuf.data(), sbuf.size());
*/
	msgpack::zc_safe_input_object obj(pk);

	msgpack::output_object_array_type cpk;

	obj.convert(cpk);
//	delete[] obj.via.map.ptr;



	for (auto it = cpk.begin(); it != cpk.end(); ++it) {
		//std::cout << it->via. << ":" << it->second << std::endl;
		std::cout << (int)it->type << std::endl;
		break;
	}

	return 0;
}
