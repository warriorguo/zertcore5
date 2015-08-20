/*
 * FileManager.cpp
 *
 *  Created on: 2015年4月27日
 *      Author: Administrator
 */

#include <fstream>
#include <libgen.h>
#include <log/Log.h>

#include "FileManager.h"

namespace zertcore { namespace utils {

bool FileManager::
getContent(const string& filename, SharedBuffer& buffer) {
	std::ifstream file(filename.c_str(), std::ifstream::binary | std::ifstream::in);

	if (!file.is_open()) {
		ZCLOG(ERROR) << "Try to read " << filename << " failed" << End;
		return false;
	}

	file.seekg(0, file.end);
	int length = file.tellg();
	file.seekg(0, file.beg);

	buffer.resize(length);
	buffer.setSize(0);

	file.read((char *)buffer.data(), buffer.capacity());
	file.close();

	return true;
}

bool FileManager::
saveContent(const string& filename, const SharedBuffer& buffer) {
	vector<char> path;
	path.assign(filename.begin(), filename.end());

	dirname(&path[0]);
	mkdir(&path[0], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	std::ofstream file(filename.c_str(), std::ofstream::binary | std::ofstream::out);

	if (!file.is_open()) {
		ZCLOG(ERROR) << "Try to write " << filename << " failed" << End;
		return false;
	}

	file.write((const char *)buffer.data(), buffer.size());
	file.close();

	return true;
}

}}


