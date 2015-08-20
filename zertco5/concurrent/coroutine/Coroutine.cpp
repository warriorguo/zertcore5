/*
 * Coroutine.cpp
 *
 *  Created on: 2015年7月31日
 *      Author: Administrator
 */

#include "Coroutine.h"

namespace zertcore { namespace concurrent { namespace co {

FCell::ptr FCell::
create() {
	return new FCell;
}

void FCell::
release() {
	delete this;
}

}}}

namespace zertcore { namespace concurrent {

bool Coroutine::
make(co::FCell::ptr cell, void (*f)(co::FCell::ptr, void *), co::FCell::ptr fc, void * params) {
	if (!cell) return false;

	ZC_ASSERT(-1 != getcontext(&cell->context));
	cell->context.uc_stack.ss_sp = cell->buffer;
	cell->context.uc_stack.ss_size = ZC_COROUTINE_BUFFER;
	cell->context.uc_link = &current_ctx_;

	makecontext(&cell->context, (void (*)())f, 2, fc, params);
	return true;
}

bool Coroutine::
make(co::FCell::ptr cell) {
	if (!cell) return false;

	ZC_ASSERT(-1 != getcontext(&cell->context));
	cell->context.uc_stack.ss_sp = cell->buffer;
	cell->context.uc_stack.ss_size = ZC_COROUTINE_BUFFER;
	cell->context.uc_link = &current_ctx_;

	return true;
}

void Coroutine::
jumpTo(co::FCell::ptr current) {
	ZC_ASSERT(-1 != swapcontext(&current_ctx_, &current->context));
}

void Coroutine::
jumpBack(co::FCell::ptr current) {
	current->flag = true;
	ZC_ASSERT(-1 != swapcontext(&current->context, &current_ctx_));
}

}}

