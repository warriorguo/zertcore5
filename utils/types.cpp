/*
 * types.cpp
 *
 *  Created on: 2014Äê12ÔÂ11ÈÕ
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


