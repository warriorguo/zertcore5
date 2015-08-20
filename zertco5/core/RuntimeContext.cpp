/*
 * RuntimeContext.cpp
 *
 *  Created on: 2015年4月7日
 *      Author: Administrator
 */

#include "RuntimeContext.h"

namespace zertcore{

RuntimeContext& RuntimeContext::
operator= (const RuntimeContext& rc) {
	if (!error)
		error = rc.error;
	return *this;
}

RuntimeContext::
operator bool() const {
	return error;
}

void RuntimeContext::
clear() {
	error.clear();
}

}


