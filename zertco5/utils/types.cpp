/*
 * types.cpp
 *
 *  Created on: 2014Äê12ÔÂ11ÈÕ
 *      Author: Administrator
 */
#include <pch.h>
#include "types.h"

namespace zertcore {

Error& Error::setError(const string& msg) {
	type = ERROR;
	code = error::UNKNOWN;
	message = msg;

	return *this;
}

Error& Error::setError(const error_code& c) {
	type = (c == error::NONE)? NONE: ERROR;
	code = c;

	return *this;
}

Error& Error::setError(const error_code& c, const string& msg) {
	type = (c == error::NONE)? NONE: ERROR;
	code = c;
	message = msg;

	return *this;
}

Error::operator bool() const {
	return type == ERROR;
}

}


namespace zertcore {

RunningContext& RunningContext::operator= (const RunningContext& rc) {
	if (!error)
		error = rc.error;
	return *this;
}

}

