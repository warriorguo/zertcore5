/*
 * context3.cpp
 *
 *  Created on: 2015年8月4日
 *      Author: Administrator
 */

#include <cstdio>
#include <ucontext.h>
#include <assert.h>
#include <pthread.h>
#include <boost/function.hpp>

void run(void * params);

struct ContextState
{
	enum {
		BUFF_SIZE = 8192,
	};

	boost::function<void (ContextState*)> handler;

	int id;
	ucontext_t ctx;
	ucontext_t& current_ctx;
	char buffer[BUFF_SIZE];

	template <class Handler>
	ContextState(Handler h, ucontext_t& c) : id(0), current_ctx(c) {
		handler = h;
	}

	void yeild() {
		swapcontext(&ctx, &current_ctx);
	}

	void resume() {
		swapcontext(&current_ctx, &ctx);
	}

	void operator () () {
		getcontext(&ctx);
		ctx.uc_stack.ss_sp = buffer;
		ctx.uc_stack.ss_size = BUFF_SIZE;
		ctx.uc_link = &current_ctx;

		makecontext(&ctx, (void (*)())run, 1, this);
		swapcontext(&current_ctx, &ctx);
	}
};

void run(void * params) {
	ContextState* state = (ContextState*)params;
	state->handler(state);
}


void f1(ContextState * state, int i) {
	state->yeild();
	assert(i == state->id);
}

void f2(ContextState * state, int i) {
	f1(state, i);
	assert(i == state->id);
}

void f3(ContextState * state, int i) {
	f2(state, i);
	assert(i == state->id);
}

void f4(ContextState * state, int i) {
	f3(state, i);
	assert(i == state->id);
}

void f5(ContextState * state, int i) {
	f4(state, i);
	assert(i == state->id);
}

void f6(ContextState * state, int i) {
	f5(state, i);
	assert(i == state->id);
}

void f7(ContextState * state, int i) {
	f6(state, i);
	assert(i == state->id);
}

void f8(ContextState * state, int i) {
	f7(state, i);
	assert(i == state->id);
}

void f9(ContextState * state, int i) {
	f8(state, i);
	assert(i == state->id);
}

void f10(ContextState * state, int i) {
	f9(state, i);
	assert(i == state->id);
}

#define N 10000

void* work(void *) {
	ContextState* states[N];
	ucontext_t current_ctx;

	for (int i = 0; i < N; ++i) {
		states[i] = new ContextState([i] (ContextState * state) {
			state->id = i;

			f10(state, i);
			// state->yeild();
			assert(i == state->id);
		}, current_ctx);
	}

	for (int i = 0; i < N; ++i) {
		(*states[i])();
	}

	for (int i = 0; i < N; ++i) {
		states[i]->resume();
	}

	return nullptr;

}

int main() {
	pthread_t pids[10];
	for (uint i = 0; i < 3; ++i) {
		pthread_t pid = 0;
		pthread_create(&pid, nullptr, &work, nullptr);
		pids[i] = pid;
	}

	for (uint i = 0; i < 3; ++i)
		pthread_join(pids[i], nullptr);

	return 0;
}

