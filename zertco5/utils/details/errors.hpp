/*
 * errors.hpp
 *
 *  Created on: 2015��1��14��
 *      Author: Administrator
 */

#ifndef ZERTCORE_ERRORS_HPP_
#define ZERTCORE_ERRORS_HPP_

#define ZC_ERROR_DEF(name, code)			namespace zertcore { namespace error {const static Error name(code, #name);}}

/**
 *
 * Default Error Defines
 *
 */

ZC_ERROR_DEF(NONE,				0);

ZC_ERROR_DEF(UNKNOWN,			1);

ZC_ERROR_DEF(TIME_OUT,			403);

ZC_ERROR_DEF(NOT_FOUND,			404);

ZC_ERROR_DEF(SYS_FAULT,			500);

ZC_ERROR_DEF(INIT_FAILED,		510);

ZC_ERROR_DEF(SERIALIZE_FAILED,	530);

#endif /* ERRORS_HPP_ */
