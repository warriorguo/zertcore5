/*
 * FileManager.h
 *
 *  Created on: 2015年4月27日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_FILE_FILEMANAGER_H_
#define ZERTCORE_UTILS_FILE_FILEMANAGER_H_

#include <pch.h>
#include <utils/types.h>

#include <utils/buffer/SharedBuffer.h>

namespace zertcore { namespace utils {

/**
 * FileManager
 */
class FileManager
{
public:
	static bool getContent(const string& filename, SharedBuffer& buffer);
	static bool saveContent(const string& filename, const SharedBuffer& buffer);
};

}}


#endif /* UTILS_FILE_FILEMANAGER_H_ */
