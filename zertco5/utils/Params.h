/*
 * Params.h
 *
 *  Created on: 2015年2月1日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_PARAMS_H_
#define ZERTCORE_UTILS_PARAMS_H_

#include <pch.h>

namespace zertcore { namespace utils {

/**
template <typename P0 = void,
		typename P1 = void,
		typename P2 = void,
		typename P3 = void,
		typename P4 = void>
struct Params;
*/

template <typename Handle>
struct Params;

namespace details {

template <typename T>
struct make_value
{
#ifndef ZC_ENABLE_PARAMS_REF
	typedef typename remove_reference<T>::type
											type;
#else
	typedef T								type;
#endif
};

#define ZC_V(X)					typename make_value<X >::type

/**
 * set the max params amount for the concurrent to 9
 */
template <typename P0, typename P1, typename P2, typename P3,
typename P4, typename P5, typename P6, typename P7, typename P8>
struct Params_v9
{
	enum {
		args								= 9,
	};

	ZC_V(P0)					p1;
	ZC_V(P1)					p2;
	ZC_V(P2)					p3;
	ZC_V(P3)					p4;
	ZC_V(P4)					p5;
	ZC_V(P5)					p6;
	ZC_V(P6)					p7;
	ZC_V(P7)					p8;
	ZC_V(P8)					p9;

	Params_v9(P0 __p1, P1 __p2, P2 __p3, P3 __p4, P4 __p5, P5 __p6, P6 __p7, P7 __p8, P8 __p9) :
		p1(__p1), p2(__p2), p3(__p3), p4(__p4), p5(__p5), p6(__p6), p7(__p7), p8(__p8), p9(__p9) {}
};

template <typename P0, typename P1, typename P2, typename P3,
typename P4, typename P5, typename P6, typename P7>
struct Params_v8
{
	enum {
		args								= 8,
	};

	ZC_V(P0)					p1;
	ZC_V(P1)					p2;
	ZC_V(P2)					p3;
	ZC_V(P3)					p4;
	ZC_V(P4)					p5;
	ZC_V(P5)					p6;
	ZC_V(P6)					p7;
	ZC_V(P7)					p8;

	Params_v8(P0 __p1, P1 __p2, P2 __p3, P3 __p4, P4 __p5, P5 __p6, P6 __p7, P7 __p8) :
		p1(__p1), p2(__p2), p3(__p3), p4(__p4), p5(__p5), p6(__p6), p7(__p7), p8(__p8) {}
};

template <typename P0, typename P1, typename P2, typename P3,
typename P4, typename P5, typename P6>
struct Params_v7
{
	enum {
		args								= 7,
	};

	ZC_V(P0)					p1;
	ZC_V(P1)					p2;
	ZC_V(P2)					p3;
	ZC_V(P3)					p4;
	ZC_V(P4)					p5;
	ZC_V(P5)					p6;
	ZC_V(P6)					p7;

	Params_v7(P0 __p1, P1 __p2, P2 __p3, P3 __p4, P4 __p5, P5 __p6, P6 __p7) :
		p1(__p1), p2(__p2), p3(__p3), p4(__p4), p5(__p5), p6(__p6), p7(__p7) {}
};

template <typename P0, typename P1, typename P2, typename P3,
typename P4, typename P5>
struct Params_v6
{
	enum {
		args								= 6,
	};

	ZC_V(P0)					p1;
	ZC_V(P1)					p2;
	ZC_V(P2)					p3;
	ZC_V(P3)					p4;
	ZC_V(P4)					p5;
	ZC_V(P5)					p6;

	Params_v6(P0 __p1, P1 __p2, P2 __p3, P3 __p4, P4 __p5, P5 __p6) :
		p1(__p1), p2(__p2), p3(__p3), p4(__p4), p5(__p5), p6(__p6) {}
};

template <typename P0, typename P1, typename P2, typename P3, typename P4>
struct Params_v5
{
	enum {
		args								= 5,
	};

	ZC_V(P0)					p1;
	ZC_V(P1)					p2;
	ZC_V(P2)					p3;
	ZC_V(P3)					p4;
	ZC_V(P4)					p5;

	Params_v5(P0 __p1, P1 __p2, P2 __p3, P3 __p4, P4 __p5) :
		p1(__p1), p2(__p2), p3(__p3), p4(__p4), p5(__p5) {}
};

template <typename P0, typename P1, typename P2, typename P3>
struct Params_v4
{
	enum {
		args								= 4,
	};

	ZC_V(P0)					p1;
	ZC_V(P1)					p2;
	ZC_V(P2)					p3;
	ZC_V(P3)					p4;

	Params_v4(P0 __p1, P1 __p2, P2 __p3, P3 __p4) :
		p1(__p1), p2(__p2), p3(__p3), p4(__p4) {}
};

template <typename P0, typename P1, typename P2>
struct Params_v3
{
	enum {
		args								= 3,
	};

	ZC_V(P0)					p1;
	ZC_V(P1)					p2;
	ZC_V(P2)					p3;

	Params_v3(P0 __p1, P1 __p2, P2 __p3) :
		p1(__p1), p2(__p2), p3(__p3) {}
};

template <typename P0, typename P1>
struct Params_v2
{
	enum {
		args								= 2,
	};

	ZC_V(P0)					p1;
	ZC_V(P1)					p2;

	Params_v2(P0 __p1, P1 __p2) :
		p1(__p1), p2(__p2) {}
};

template <typename P0>
struct Params_v1
{
	enum {
		args								= 1,
	};

	ZC_V(P0)					p1;

	Params_v1(P0 __p1) :
		p1(__p1) {}
};


struct Params_v0
{
	enum {
		args								= 0,
	};
};

#undef ZC_V

}

template <typename R>
struct Params<R ()> : public details::Params_v0 {};

template <typename R, typename P0>
struct Params<R (P0)> : public details::Params_v1<P0> {
	Params(P0 __p1) : details::Params_v1<P0>(__p1) {}
};

template <typename R, typename P0, typename P1>
struct Params<R (P0, P1)> : public details::Params_v2<P0, P1> {
	Params(P0 __p1, P1 __p2) : details::Params_v2<P0, P1>(__p1,__p2) {}
};

template <typename R, typename P0, typename P1, typename P2>
struct Params<R (P0, P1, P2)> : public details::Params_v3<P0, P1, P2> {
	Params(P0 __p1, P1 __p2, P2 __p3) :
		details::Params_v3<P0, P1, P2>(__p1,__p2,__p3) {}
};

template <typename R, typename P0, typename P1, typename P2, typename P3>
struct Params<R (P0, P1, P2, P3)> : public details::Params_v4<P0, P1, P2, P3> {
	Params(P0 __p1, P1 __p2, P2 __p3, P3 __p4) :
		details::Params_v4<P0, P1, P2, P3>(__p1,__p2,__p3,__p4) {}
};

template <typename R, typename P0, typename P1, typename P2, typename P3, typename P4>
struct Params<R (P0, P1, P2, P3, P4)> : public details::Params_v5<P0, P1, P2, P3, P4> {
	Params(P0 __p1, P1 __p2, P2 __p3, P3 __p4, P4 __p5) :
		details::Params_v5<P0, P1, P2, P3, P4>(__p1,__p2,__p3,__p4,__p5) {}
};

template <typename R, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5>
struct Params<R (P0, P1, P2, P3, P4, P5)> : public details::Params_v6<P0, P1, P2, P3, P4, P5> {
	Params(P0 __p1, P1 __p2, P2 __p3, P3 __p4, P4 __p5, P5 __p6) :
		details::Params_v6<P0, P1, P2, P3, P4, P5>(__p1,__p2,__p3,__p4,__p5,__p6) {}
};

template <typename R, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
struct Params<R (P0, P1, P2, P3, P4, P5, P6)> : public details::Params_v7<P0, P1, P2, P3, P4, P5, P6> {
	Params(P0 __p1, P1 __p2, P2 __p3, P3 __p4, P4 __p5, P5 __p6, P6 __p7) :
		details::Params_v7<P0, P1, P2, P3, P4, P5, P6>(__p1,__p2,__p3,__p4,__p5,__p6,__p7) {}
};

template <typename R, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
struct Params<R (P0, P1, P2, P3, P4, P5, P6, P7)> : public details::Params_v8<P0, P1, P2, P3, P4, P5, P6, P7> {
	Params(P0 __p1, P1 __p2, P2 __p3, P3 __p4, P4 __p5, P5 __p6, P6 __p7, P7 __p8) :
		details::Params_v8<P0, P1, P2, P3, P4, P5, P6, P7>(__p1,__p2,__p3,__p4,__p5,__p6,__p7,__p8) {}
};

template <typename R, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
struct Params<R (P0, P1, P2, P3, P4, P5, P6, P7, P8)> : public details::Params_v9<P0, P1, P2, P3, P4, P5, P6, P7, P8> {
	Params(P0 __p1, P1 __p2, P2 __p3, P3 __p4, P4 __p5, P5 __p6, P6 __p7, P7 __p8, P8 __p9) :
		details::Params_v9<P0, P1, P2, P3, P4, P5, P6, P7, P8>(__p1,__p2,__p3,__p4,__p5,__p6,__p7,__p8,__p9) {}
};

template <typename HANDLER>
typename function<HANDLER>::result_type
callWithParams(const function<HANDLER>& handler) {
	return handler();
}

template <typename HANDLER>
typename function<HANDLER>::result_type
callWithParams(const function<HANDLER>& handler, details::Params_v0& params) {
	return handler();
}

template <typename HANDLER, typename P0>
typename function<HANDLER>::result_type
callWithParams(const function<HANDLER>& handler, details::Params_v1<P0>& params) {
	return handler(params.p1);
}

template <typename HANDLER, typename P0, typename P1>
typename function<HANDLER>::result_type
callWithParams(const function<HANDLER>& handler, details::Params_v2<P0, P1>& params) {
	return handler(params.p1, params.p2);
}

template <typename HANDLER, typename P0, typename P1, typename P2>
typename function<HANDLER>::result_type
callWithParams(const function<HANDLER>& handler, details::Params_v3<P0, P1, P2>& params) {
	return handler(params.p1, params.p2, params.p3);
}

template <typename HANDLER, typename P0, typename P1, typename P2, typename P3>
typename function<HANDLER>::result_type
callWithParams(const function<HANDLER>& handler, details::Params_v4<P0, P1, P2, P3>& params) {
	return handler(params.p1, params.p2, params.p3, params.p4);
}

template <typename HANDLER, typename P0, typename P1, typename P2, typename P3, typename P4>
typename function<HANDLER>::result_type
callWithParams(const function<HANDLER>& handler, details::Params_v5<P0, P1, P2, P3, P4>& params) {
	return handler(params.p1, params.p2, params.p3, params.p4, params.p5);
}

template <typename HANDLER, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5>
typename function<HANDLER>::result_type
callWithParams(const function<HANDLER>& handler, details::Params_v6<P0, P1, P2, P3, P4, P5>& params) {
	return handler(params.p1, params.p2, params.p3, params.p4, params.p5, params.p6);
}

template <typename HANDLER, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
typename function<HANDLER>::result_type
callWithParams(const function<HANDLER>& handler, details::Params_v7<P0, P1, P2, P3, P4, P5, P6>& params) {
	return handler(params.p1, params.p2, params.p3, params.p4, params.p5, params.p6, params.p7);
}

template <typename HANDLER, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
typename function<HANDLER>::result_type
callWithParams(const function<HANDLER>& handler, details::Params_v8<P0, P1, P2, P3, P4, P5, P6, P7>& params) {
	return handler(params.p1, params.p2, params.p3, params.p4, params.p5, params.p6, params.p7, params.p8);
}

template <typename HANDLER, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
typename function<HANDLER>::result_type
callWithParams(const function<HANDLER>& handler, details::Params_v9<P0, P1, P2, P3, P4, P5, P6, P7, P8>& params) {
	return handler(params.p1, params.p2, params.p3, params.p4, params.p5, params.p6, params.p7, params.p8, params.p9);
}


}}


#endif /* PARAMS_H_ */
