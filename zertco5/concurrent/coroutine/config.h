/*
 * config.h
 *
 *  Created on: 2015年7月31日
 *      Author: Administrator
 */

#ifndef ZERTCORE_CONCURRENT_COROUTINE_CONFIG_H_
#define ZERTCORE_CONCURRENT_COROUTINE_CONFIG_H_

#include <pch.h>
#include <utils/types.h>
#include <object/PoolObject.h>

#ifndef ZC_COROUTINE_BUFFER
#  define ZC_COROUTINE_BUFFER				81920
#endif

namespace zertcore { namespace concurrent { namespace co {
using namespace zertcore::object;

typedef ucontext_t							context_type;

struct FCell
{
	typedef FCell*							ptr;

	static ptr create();
	void release();

	context_type			context;
	byte					buffer[ZC_COROUTINE_BUFFER];

	bool					flag{false};
	bool					finished{false};

	operator bool () const { return flag; }
};
struct Cell
{
	context_type			context;
	bool					flag{false};

	operator bool () const { return flag; }
};

}}}

namespace zertcore {

typedef concurrent::co::Cell				cocell_type;
typedef concurrent::co::FCell				cofcell_type;

}


#endif /* CONCURRENT_COROUTINE_CONFIG_H_ */
