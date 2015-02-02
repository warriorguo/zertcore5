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
	P0							_1;
	P1							_2;
	P2							_3;
	P3							_4;
	P4							_5;

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
			_1(p._1),_2(p._2),_3(p._3),_4(p._4),_5(p._5) {}
#ifndef ZC_COMPILE
	explicit Params(T0 __1, T1 __2, T2 __3, T3 __4, T4 __5) :
			_1(__1), _2(__2), _3(__3), _4(__4), _5(__5) {}
#else
#  include "details/Params5Details.ipp"
#endif

	template <typename HANDLER>
	typename function<HANDLER>::result_type invokeWith(const function<HANDLER>& handler) const {
		return handler(_1, _2, _3, _4, _5);
	}
};

template <typename P0, typename P1, typename P2, typename P3>
struct Params<P0, P1, P2, P3, void>
{
	P0							_1;
	P1							_2;
	P2							_3;
	P3							_4;

	typedef typename remove_const<typename remove_reference<P0>::type>::type
											T0;
	typedef typename remove_const<typename remove_reference<P1>::type>::type
											T1;
	typedef typename remove_const<typename remove_reference<P2>::type>::type
											T2;
	typedef typename remove_const<typename remove_reference<P3>::type>::type
											T3;

	explicit Params(const Params& p) :
			_1(p._1),_2(p._2),_3(p._3),_4(p._4) {}
#ifndef ZC_COMPILE
	explicit Params(T0 __1, T1 __2, T2 __3, T3 __4) :
			_1(__1), _2(__2), _3(__3), _4(__4) {}
#else
#  include "details/Params4Details.ipp"
#endif

	template <typename HANDLER>
	typename function<HANDLER>::result_type invokeWith(const function<HANDLER>& handler) const {
		return handler(_1, _2, _3, _4);
	}
};

template <typename P0, typename P1, typename P2>
struct Params<P0, P1, P2, void, void>
{
	P0							_1;
	P1							_2;
	P2							_3;

	typedef typename remove_const<typename remove_reference<P0>::type>::type
											T0;
	typedef typename remove_const<typename remove_reference<P1>::type>::type
											T1;
	typedef typename remove_const<typename remove_reference<P2>::type>::type
											T2;

	explicit Params(const Params& p) :
			_1(p._1),_2(p._2),_3(p._3) {}
#ifndef ZC_COMPILE
	explicit Params(T0 __1, T1 __2, T2 __3) :
			_1(__1), _2(__2), _3(__3) {}
#else
#  include "details/Params3Details.ipp"
#endif

	template <typename HANDLER>
	typename function<HANDLER>::result_type invokeWith(const function<HANDLER>& handler) const {
		return handler(_1, _2, _3);
	}
};

template <typename P0, typename P1>
struct Params<P0, P1, void, void, void>
{
	P0							_1;
	P1							_2;

	typedef typename remove_const<typename remove_reference<P0>::type>::type
											T0;
	typedef typename remove_const<typename remove_reference<P1>::type>::type
											T1;

	explicit Params(const Params& p) :
			_1(p._1),_2(p._2) {}
#ifndef ZC_COMPILE
	explicit Params(T0 __1, T1 __2) :
			_1(__1), _2(__2) {}
#else
#  include "details/Params2Details.ipp"
#endif

	template <typename HANDLER>
	typename function<HANDLER>::result_type invokeWith(const function<HANDLER>& handler) const {
		return handler(_1, _2);
	}
};

template <typename P0>
struct Params<P0, void, void, void, void>
{
	P0							_1;

	typedef typename remove_const<typename remove_reference<P0>::type>::type
											T0;

	explicit Params(const Params& p) :
			_1(p._1) {}
#ifndef ZC_COMPILE
	explicit Params(T0 __1) :
			_1(__1) {}
#else
#  include "details/Params1Details.ipp"
#endif

	template <typename HANDLER>
	typename function<HANDLER>::result_type invokeWith(const function<HANDLER>& handler) const {
		handler(_1);
	}
};

template <>
struct Params<void, void, void, void, void>
{
	template <typename HANDLER>
	typename function<HANDLER>::result_type invokeWith(const function<HANDLER>& handler) const {
		return handler();
	}
};

}}


#endif /* PARAMS_H_ */
