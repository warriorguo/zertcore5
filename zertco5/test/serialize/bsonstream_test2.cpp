/*
 * bsonstream_test2.cpp
 *
 *  Created on: 2015年9月10日
 *      Author: Administrator
 */

#include <zertcore.h>


using namespace zertcore;
using namespace zertcore::serialization;
using namespace zertcore::db::mongodb::serialization;

int main() {
	Serializer<BSONIStream> in;
	Unserializer<BSONOStream> out;

	map<key_type, u32> m1, m2;
	m1["haha"] = 1;
	m1["hehe"] = 2;

	in["show1"] & 1;
	in["show2"]["me"] & 2;
	in["show3"]["me"]["the"] & 3;
	in["show4"]["me"]["the"]["money"] & 4;

	in["black1"] & "black";
	in["black2"]["sheep"] & "sheep";
	in["black3"]["sheep"]["wall"] & "wall";

	in["laugth"]["for"]["ever"] & m1;

	SharedBuffer sb = in.buffer();
	::printf("%s\n", sb.data());
	out.buffer(sb);

	u32 a, b, c, d;
	string s1, s2, s3;

	out["show1"] & a;
	out["show2"]["me"] & b;
	out["show3"]["me"]["the"] & c;
	out["show4"]["me"]["the"]["money"] & d;

	out["black1"] & s1;
	out["black2"]["sheep"] & s2;
	out["black3"]["sheep"]["wall"] & s3;

	out["laugth"]["for"]["ever"] & m2;

	::printf("%d %d %d %d\n%s %s %s\n", a, b, c, d, s1.c_str(), s2.c_str(), s3.c_str());

	for (auto it = m2.begin(); it != m2.end(); ++it) {
		::printf("k:%s v:%u\n", it->first.c_str(), it->second);
	}

	return 0;
}
