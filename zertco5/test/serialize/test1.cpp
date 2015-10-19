/*
 * test1.cpp
 *
 *  Created on: 2014-4-29
 *      Author: Administrator
 */
#include <pch.h>
#include <serialize/Serializer.h>

#include <utils/msgpack/MsgPackStream.h>

using namespace zertcore;
using namespace zertcore::utils;

typedef serialization::Serializer<messagepack::MsgPackIStream>
											iachiver_type;

void f(iachiver_type i) {}

int main() {
	iachiver_type i;
	f(i);
	return 0;
}
