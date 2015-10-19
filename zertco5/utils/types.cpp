/*
 * types.cpp
 *
 *  Created on: 2014��12��11��
 *      Author: Administrator
 */
#include <pch.h>
#include "types.h"

namespace zertcore {

Error& Error::setError(const string& msg) {
	*this = error::UNKNOWN;
	message = msg;

	return *this;
}

Error& Error::setError(const error_code& c) {
	type = (c == NONE)? NONE: ERROR;
	code = c;

	return *this;
}

Error& Error::setError(const error_code& c, const string& msg) {
	type = (c == NONE)? NONE: ERROR;
	code = c;
	message = msg;

	return *this;
}

Error::operator bool() const {
	return type == ERROR;
}

void Error::
clear() {
	type = NONE;
	code = 0;
	message.clear();
}

}


namespace zertcore {

}

