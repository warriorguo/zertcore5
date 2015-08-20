/*
 * tags_map_test1.cpp
 *
 *  Created on: 2015-2-17
 *      Author: Administrator
 */

#include <utils/TagsMap.h>

using namespace zertcore;
using namespace zertcore::utils;

int main() {
	typedef TagsMap<u32, string, int>		tags_map_type;
	tags_map_type tags;

	tags.insert(32, "show");
	tags.insert(84, "me", 1000);
	tags.insert(139, 1001);
	auto it = tags.insert(233);
	it.tag("the");
	it.tag(12);

	tags.erase(it);
	tags.erase(12);

	return 0;
}
