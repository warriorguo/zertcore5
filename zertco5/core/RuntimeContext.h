/*
 * RuntimeContext.h
 *
 *  Created on: 2015年4月7日
 *      Author: Administrator
 */

#ifndef ZERTCORE_CORE_RUNTIMECONTEXT_H_
#define ZERTCORE_CORE_RUNTIMECONTEXT_H_

#include <pch.h>
#include <utils/types.h>
#include <object/PoolObject.h>

namespace zertcore{

/**
 * RuntimeContext
 */
struct RuntimeContext : public object::PoolObject<RuntimeContext>
{
	Error						error;

	RuntimeContext& operator= (const RuntimeContext& rc);
	operator bool() const;

	void clear();

	ZC_TO_STRING("error_code" << error.code << "error_msg" << error.message);
};

}


#endif /* CORE_RUNTIMECONTEXT_H_ */
