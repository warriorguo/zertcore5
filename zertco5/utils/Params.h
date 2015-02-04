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

template <typename P0 = void,
		typename P1 = void,
		typename P2 = void,
		typename P3 = void,
		typename P4 = void>
struct Params;
/**
 * set the max params amount for the concurrent to 5
 */
template <typename P0, typename P1, typename P2, typename P3, typename P4>
struct Params
{
	P0							p1;
	P1							p2;
	P2							p3;
	P3							p4;
	P4							p5;
};

template <typename HANDLER, typename P0, typename P1, typename P2, typename P3, typename P4>
typename function<HANDLER>::result_type
callWithParams(const function<HANDLER>& handler, Params<P0, P1, P2, P3, P4>& params) {
	return handler(params.p1, params.p2, params.p3, params.p4, params.p5);
}

template <typename P0, typename P1, typename P2, typename P3>
struct Params<P0, P1, P2, P3, void>
{
	P0							p1;
	P1							p2;
	P2							p3;
	P3							p4;
};

template <typename HANDLER, typename P0, typename P1, typename P2, typename P3>
typename function<HANDLER>::result_type
callWithParams(const function<HANDLER>& handler, Params<P0, P1, P2, P3>& params) {
	return handler(params.p1, params.p2, params.p3, params.p4);
}

template <typename P0, typename P1, typename P2>
struct Params<P0, P1, P2, void, void>
{
	P0							p1;
	P1							p2;
	P2							p3;
};

template <typename HANDLER, typename P0, typename P1, typename P2>
typename function<HANDLER>::result_type
callWithParams(const function<HANDLER>& handler, Params<P0, P1, P2>& params) {
	return handler(params.p1, params.p2, params.p3);
}

template <typename P0, typename P1>
struct Params<P0, P1, void, void, void>
{
	P0							p1;
	P1							p2;
};

template <typename HANDLER, typename P0, typename P1>
typename function<HANDLER>::result_type
callWithParams(const function<HANDLER>& handler, Params<P0, P1>& params) {
	return handler(params.p1, params.p2);
}

template <typename P0>
struct Params<P0, void, void, void, void>
{
	P0							p1;
};

template <typename HANDLER, typename P0>
typename function<HANDLER>::result_type
callWithParams(const function<HANDLER>& handler, Params<P0>& params) {
	return handler(params.p1);
}

template <>
struct Params<void, void, void, void, void>
{
};

template <typename HANDLER>
typename function<HANDLER>::result_type
callWithParams(const function<HANDLER>& handler, Params<>& params) {
	return handler();
}

typedef Params<>							default_params_type;

}}


#endif /* PARAMS_H_ */
