/*
 * types.cpp
 *
 *  Created on: 2014��12��11��
 *      Author: Administrator
 */
#include <pch.h>
#include <utils/types.h>

namespace zertcore {

Error& Error::setError(const string& msg) {
	type = ERROR;
	message = msg;

	return *this;
}

Error::operator bool() const {
	return type != ERROR;
}

}


