/*
 * Coroutine.h
 *
 *  Created on: 2015年7月31日
 *      Author: Administrator
 */

#ifndef ZERTCORE_CONCURRENT_COROUTINE_COROUTINE_H_
#define ZERTCORE_CONCURRENT_COROUTINE_COROUTINE_H_

#include <thread/ThreadSingleton.h>
#include "config.h"

namespace zertcore { namespace concurrent {
using namespace zertcore::object;
}}

namespace zertcore { namespace concurrent {

/**
 * Coroutine
 */
class Coroutine :
		public ThreadSingleton<Coroutine>
{
public:
	bool make(co::FCell::ptr cell, void (*)(co::FCell::ptr, void *), co::FCell::ptr, void * params);
	bool make(co::FCell::ptr cell);

public:
	void jumpTo(co::FCell::ptr current);
	void jumpBack(co::FCell::ptr current);

public:
	co::FCell::ptr getLastFCell() {return last_fc_;}
	void setLastFCell(co::FCell::ptr fc) {last_fc_ = fc;}

private:
	co::context_type			current_ctx_;
	co::FCell::ptr				last_fc_;
};

}}


#endif /* CONCURRENT_COROUTINE_COROUTINE_H_ */
