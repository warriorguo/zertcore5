/*
 * types.h
 *
 *  Created on: 2014-4-28
 *      Author: Administrator
 */

#ifndef ZERTCORE_TYPES_H_
#define ZERTCORE_TYPES_H_

#include <pch.h>

#include <utils/details/null.hpp>

namespace zertcore {

typedef char								i8;
typedef short int							i16;
typedef int									i32;
typedef long long							i64;

typedef unsigned char						u8;
typedef unsigned short int					u16;
typedef unsigned int						u32;
typedef unsigned long long					u64;

typedef float								f32;
typedef double								f64;

}

#define ZC_ASSERT(x)						assert(x)

namespace zertcore {
using ::std::string;
using ::std::list;
using ::std::deque;
using ::std::map;
using ::std::multimap;
using ::std::unordered_map;
using ::std::unordered_multimap;
using ::std::vector;
using ::std::set;
using ::std::multiset;
using ::std::unordered_set;
using ::std::unordered_multiset;
using ::std::pair;
using ::std::move;
}

namespace zertcore {
typedef u64									uuid_t;
}

namespace zertcore {

/**
 * Flags
 */
const static struct __BEGIN{}				Begin;
const static struct __END {}				End;
const static struct __ALL {}				All;
const static struct __ONE {}				One;
const static struct __OTHER {}				Other;
const static struct __NOW {}				Now;

const static struct __READWRITE {}			ReadWrite;
const static struct __READONLY {}			ReadOnly;

}

namespace zertcore {

/**
 * block type
 */
enum block_type {
	BLOCK									= 1,
	NON_BLOCK								= 2,
};

/**
 * every thread got its owns index
 */
typedef u32									tid_type;

typedef detail::spinlock					spinlock_type;
typedef detail::spinlock::scoped_lock		spinlock_guard_type;

}

#include "details/errors.hpp"

namespace zertcore {

typedef u32									error_code;

/**
 * Error
 */
struct Error
{
	enum error_type {
		NONE					= 0,
		ERROR					= 1,
	};
	error_type					type;

	error_code					code;
	string						message;

	explicit Error() : type(NONE), code(error::NONE) {}
	explicit Error(const string& msg) : type(ERROR), code(error::NONE), message(msg) {}

public:
	Error& setError(const string& msg);
	Error& setError(const error_code& c);
	Error& setError(const error_code& c, const string& msg);

	operator bool() const;
};

/**
 * Parameter
 */
struct Parameter
{
};


#ifndef CONTEXT_PARAMS_AMOUNT
#  define CONTEXT_PARAMS_AMOUNT				4
#endif

/**
 * RunningContext
 */
struct RunningContext
{
	Error						error;
	array<Parameter, CONTEXT_PARAMS_AMOUNT>
								params;

	RunningContext& operator= (const RunningContext& rc);
};

/**
 * Remote
 */
struct Remote
{
	string						host;
	u32							port;

	Remote(const string& h, const u32& p): host(h), port(p) {}
};


#define ZC_MAKE_U16(a,b)		((u8(b) << 8) | u8(a))
#define ZC_MAKE_U32(a,b,c,d)	(u8(a) | (u8(b) << 8) | (u8(c) << 16) | (u8(d) << 24))
#define ZC_MAKE_U64(a,b,c,d,e,f,g,h) \
								(u8(a) | (u8(b) << 8) | (u8(c) << 16) | (u8(d) << 24)) |\
								((u8(e) << 32) | (u8(f) << 40) | (u8(g) << 48) | (u8(h) << 56))

/**
 * mainly for functions return reference values to check the validation.
 */
/**
template <typename T>
struct RetRef
{
public:
	typedef T								type;
	typedef typename remove_reference<T>::type
											noref_type;
	typedef typename add_reference<T>::type	reference_type;
	typedef typename add_pointer<noref_type>::type
											point_type;

	typedef typename add_const<reference_type>::type
											const_reference_type;
	typedef typename add_const<point_type>::type
											const_point_type;

public:
	bool						success;
	Error						error;
	point_type					value_ptr;

public:
	RetRef(reference_type v) : success(true), value_ptr(&v) {}
	RetRef(point_type pv) : success(true), value_ptr(pv) {}
	RetRef(const Null&) : success(false), value_ptr(null) {}
	RetRef(const Error& e) : success(false), error(e), value_ptr(null) {}

	operator bool() const {
		return success;
	}
	point_type operator-> () {
		return value_ptr;
	}
	const_point_type operator-> () const {
		return value_ptr;
	}
	reference_type operator* () {
		return *value_ptr;
	}
	const_reference_type operator* () const {
		return *value_ptr;
	}
};
*/

}

namespace zertcore { namespace utils {

template <typename T>
inline void clearUp(T& v);

template <typename T>
inline bool isEmpty(const T& v);

}}

#include "details/types.hpp"

#endif /* TYPES_H_ */
