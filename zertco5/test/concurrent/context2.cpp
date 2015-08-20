/*
 * context2.cpp
 *
 *  Created on: 2015年7月31日
 *      Author: Administrator
 */

#include <cstdio>
#include <ucontext.h>

ucontext_t fcm, begin_fc;

class ContextState
{
public:
	void run() {
		static int a = 123;
		::printf("in state run:%d\n", a++);
		yeild();
		::printf("resume state run:%d\n", a++);
	}
	void yeild() {
		::printf("before yeild\n");
		swapcontext(&now_fc_, &begin_fc);
		::printf("after yeild\n");
	}
	void resume() {
		swapcontext(&begin_fc, &now_fc_);
	}

private:
	ucontext_t					now_fc_;
};

void run(void * params) {
	::printf("in run\n");
	ContextState* state = (ContextState*)params;

	::printf("begin run\n");
	state->run();
	::printf("end run\n");
	swapcontext(&fcm, &begin_fc);
}

#define N	1000

int main() {
	char * buffer;
	ContextState states[N];


	for (int i = 0; i < N; ++i) {
		ContextState* state = & states[i];
		getcontext(&fcm);
		fcm.uc_stack.ss_sp = new char[8192];
		fcm.uc_stack.ss_size = 8192;
		fcm.uc_link = &begin_fc;
		makecontext(&fcm, (void (*)())run, 1, state);

		printf("jump run:%d\n", i);
		swapcontext(&begin_fc, &fcm);
	}

	printf("------------------------------------\n");

	for (int i = 0; i < N; ++i) {
		ContextState* state = & states[i];

		printf("before resume\n");
		state->resume();
		printf("after resume:%d\n", i);
	}

	::printf("ALL End\n");
	return 0;
}
