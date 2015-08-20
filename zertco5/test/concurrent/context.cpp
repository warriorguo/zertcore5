/*
 * context.cpp
 *
 *  Created on: 2015年7月29日
 *      Author: Administrator
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <boost/context/fcontext.hpp>

/**
boost::context::fcontext_t fcm,fc1,fc2;

void f1(intptr_t)
{
    std::cout<<"f1: entered"<<std::endl;
    std::cout<<"f1: call jump_fcontext( & fc1, fc2, 0)"<< std::endl;
    boost::context::jump_fcontext(&fc1,fc2,0);
    std::cout<<"f1: return"<<std::endl;
    boost::context::jump_fcontext(&fc1,fcm,0);
}

void f2(intptr_t)
{
    std::cout<<"f2: entered"<<std::endl;
    std::cout<<"f2: call jump_fcontext( & fc2, fc1, 0)"<<std::endl;
    boost::context::jump_fcontext(&fc2,fc1,0);
}

int main() {
	std::size_t size(8192);
	void* sp1(std::malloc(size));
	void* sp2(std::malloc(size));

	fc1=boost::context::make_fcontext(sp1,size,f1);
	fc2=boost::context::make_fcontext(sp2,size,f2);

	std::cout<<"main: call jump_fcontext( & fcm, fc1, 0)"<<std::endl;
	boost::context::jump_fcontext(&fcm,fc1,0);

	return 0;
}
*/

using namespace boost;

context::fcontext_t fcm, begin_fc;

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
		context::jump_fcontext(&now_fc_, begin_fc, 0);
		::printf("after yeild\n");
	}
	void resume() {
		context::jump_fcontext(&begin_fc, now_fc_, 0);
	}

private:
	context::fcontext_t			now_fc_;
};

void run(intptr_t params) {
	::printf("in run\n");
	ContextState* state = (ContextState*)params;

	::printf("begin run\n");
	state->run();
	::printf("end run\n");
	context::jump_fcontext(&fcm, begin_fc, (intptr_t)& state);
}

#define N	1000

int main() {
	const std::size_t size(8192);
	ContextState states[N];


	for (uint i = 0; i < N; ++i) {
		ContextState* state = & states[i];
		void * sp(std::malloc(size));
		fcm = context::make_fcontext(sp, size, run);

		printf("jump run:%d\n", i);
		context::jump_fcontext(&begin_fc, fcm, (intptr_t)state);
	}

	printf("------------------------------------\n");

	for (uint i = 0; i < N; ++i) {
		ContextState* state = & states[i];

		printf("before resume\n");
		state->resume();
		printf("after resume:%d\n", i);
	}

	::printf("ALL End\n");
	return 0;
}

