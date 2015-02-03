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

	typedef typename remove_const<typename remove_reference<P0>::type>::type
											T0;
	typedef typename remove_const<typename remove_reference<P1>::type>::type
											T1;
	typedef typename remove_const<typename remove_reference<P2>::type>::type
											T2;
	typedef typename remove_const<typename remove_reference<P3>::type>::type
											T3;
	typedef typename remove_const<typename remove_reference<P4>::type>::type
											T4;

	explicit Params(const Params& p) :
			p1(p.p1),p2(p.p2),p3(p.p3),p4(p.p4),p5(p.p5) {}
#ifndef ZC_COMPILE
	explicit Params(T0 _p1, T1 _p2, T2 _p3, T3 _p4, T4 _p5) :
			p1(_p1), p2(_p2), p3(_p3), p4(_p4), p5(_p5) {}
#else
#  include "details/Params5Details.ipp"
#endif

	template <typename HANDLER>
	typename function<HANDLER>::result_type invokeWith(const function<HANDLER>& handler) const {
		return handler(p1, p2, p3, p4, p5);
	}
	template <typename HANDLER>
	typename function<HANDLER>::result_type invokeWith(const function<HANDLER>& handler) {
		return handler(p1, p2, p3, p4, p5);
	}
};

template <typename P0, typename P1, typename P2, typename P3>
struct Params<P0, P1, P2, P3, void>
{
	P0							p1;
	P1							p2;
	P2							p3;
	P3							p4;

	typedef typename remove_const<typename remove_reference<P0>::type>::type
											T0;
	typedef typename remove_const<typename remove_reference<P1>::type>::type
											T1;
	typedef typename remove_const<typename remove_reference<P2>::type>::type
											T2;
	typedef typename remove_const<typename remove_reference<P3>::type>::type
											T3;

	explicit Params(const Params& p) :
			p1(p.p1),p2(p.p2),p3(p.p3),p4(p.p4) {}
#ifndef ZC_COMPILE
	explicit Params(T0 _p1, T1 _p2, T2 _p3, T3 _p4) :
			p1(_p1), p2(_p2), p3(_p3), p4(_p4) {}
#else
#  include "details/Params4Details.ipp"
#endif

	template <typename HANDLER>
	typename function<HANDLER>::result_type invokeWith(const function<HANDLER>& handler) const {
		return handler(p1, p2, p3, p4);
	}
	template <typename HANDLER>
	typename function<HANDLER>::result_type invokeWith(const function<HANDLER>& handler) {
		return handler(p1, p2, p3, p4);
	}
};

template <typename P0, typename P1, typename P2>
struct Params<P0, P1, P2, void, void>
{
	P0							p1;
	P1							p2;
	P2							p3;

	typedef typename remove_const<typename remove_reference<P0>::type>::type
											T0;
	typedef typename remove_const<typename remove_reference<P1>::type>::type
											T1;
	typedef typename remove_const<typename remove_reference<P2>::type>::type
											T2;

	explicit Params(const Params& p) :
			p1(p.p1),p2(p.p2),p3(p.p3) {}
#ifndef ZC_COMPILE
	explicit Params(T0 _p1, T1 _p2, T2 _p3) :
			p1(_p1), p2(_p2), p3(_p3) {}
#else
#  include "details/Params3Details.ipp"
#endif

	template <typename HANDLER>
	typename function<HANDLER>::result_type invokeWith(const function<HANDLER>& handler) const {
		return handler(p1, p2, p3);
	}
	template <typename HANDLER>
	typename function<HANDLER>::result_type invokeWith(const function<HANDLER>& handler) {
		return handler(p1, p2, p3);
	}
};

template <typename P0, typename P1>
struct Params<P0, P1, void, void, void>
{
	P0							p1;
	P1							p2;

	typedef typename remove_const<typename remove_reference<P0>::type>::type
											T0;
	typedef typename remove_const<typename remove_reference<P1>::type>::type
											T1;

	explicit Params(const Params& p) :
			p1(p.p1),p2(p.p2) {}
#ifndef ZC_COMPILE
	explicit Params(T0 _p1, T1 _p2) :
			p1(_p1), p2(_p2) {}
#else
#  include "details/Params2Details.ipp"
#endif

	template <typename HANDLER>
	typename function<HANDLER>::result_type invokeWith(const function<HANDLER>& handler) const {
		return handler(p1, p2);
	}
	template <typename HANDLER>
	typename function<HANDLER>::result_type invokeWith(const function<HANDLER>& handler) {
		return handler(p1, p2);
	}
};

template <typename P0>
struct Params<P0, void, void, void, void>
{
	P0							p1;

	typedef typename remove_const<typename remove_reference<P0>::type>::type
											T0;

	explicit Params(const Params& p) :
			p1(p.p1) {}
#ifndef ZC_COMPILE
	explicit Params(T0 _p1) :
			p1(_p1) {}
#else
#  include "details/Params1Details.ipp"
#endif

	template <typename HANDLER>
	typename function<HANDLER>::result_type invokeWith(const function<HANDLER>& handler) const {
		return handler(p1);
	}
	template <typename HANDLER>
	typename function<HANDLER>::result_type invokeWith(const function<HANDLER>& handler) {
		return handler(p1);
	}
};

template <>
struct Params<void, void, void, void, void>
{
	template <typename HANDLER>
	typename function<HANDLER>::result_type invokeWith(const function<HANDLER>& handler) const {
		return handler();
	}
	template <typename HANDLER>
	typename function<HANDLER>::result_type invokeWith(const function<HANDLER>& handler) {
		return handler();
	}
};

}}


#endif /* PARAMS_H_ */
