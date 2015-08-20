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

namespace zertcore {

enum {
	max_i8									= 0x7f,
	max_i16									= 0x7fff,
	max_i32									= 0x7fffffff,
	max_i64									= 0x7fffffffffffffff,

	max_u8									= 0xff,
	max_u16									= 0xffff,
	max_u32									= 0xffffffff,
	max_u64									= 0xffffffffffffffff,
};

}

#define ZC_ASSERT(x)						assert(x)

#ifndef ZC_RELEASE
#  define ZC_DEBUG_ASSERT(x)				ZC_ASSERT(x)
#else
#  define ZC_DEBUG_ASSERT(x)
#endif

namespace zertcore {
using ::std::string;
using ::std::list;
using ::std::deque;
using ::std::map;
using ::std::multimap;
using ::boost::unordered_map;
using ::boost::unordered_multimap;
using ::std::vector;
using ::std::set;
using ::std::multiset;
using ::boost::unordered_set;
using ::boost::unordered_multiset;
using ::std::pair;
using ::std::move;
using ::std::forward;
using ::std::for_each;
using ::std::auto_ptr;
}

namespace zertcore {
typedef u64									uuid_t;
typedef u8									byte;
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

struct NONE {};
struct UNIQUE {};
struct NORMAL {};

}

namespace zertcore {

struct _Traits {};

}

namespace zertcore {

template <bool flag, class Select1, class Select2>
struct BinarySwitch
{
	typedef void							type;
};

template <class Select1, class Select2>
struct BinarySwitch<true, Select1, Select2>
{
	typedef Select1							type;
};

template <class Select1, class Select2>
struct BinarySwitch<false, Select1, Select2>
{
	typedef Select2							type;
};

}

namespace zertcore {

template<typename T>
struct AssignTraits : NORMAL {};

/**
 * these 3 func are mainly for safe assign action in thread handler params
 */
template<typename T>
T assign(T& v, const UNIQUE& _);

template<typename T>
T assign(const T& v, const UNIQUE& _);

template<typename T>
T& assign(T& v, const NORMAL& _) {return v;}

template<typename T>
volatile T& assign(volatile T& v, const NORMAL& _) {return v;}

template<typename T>
const T& assign(const T& v, const NORMAL& _) {return v;}

}

#include <utils/spinlock/SpinLock.h>

namespace zertcore {

/**
 * block type defines
 */
enum block_type {
	BLOCK									= 1,
	NON_BLOCK								= 2,
};

/**
 * Priority type defines
 */
enum {
	PRIORITY_LOW							= 1,
	PRIORITY_HIGH							= 2,
};

/**
 * every thread got its owns index
 */
typedef u32									tid_type;
typedef u32									priority_type;

typedef utils::SpinLock						spinlock_type;
typedef utils::LockGuard<spinlock_type>		spinlock_guard_type;

}

namespace zertcore {

/**
 * Unserializable Serializable flags
 */

struct SerializableBase {};
struct UnserializableBase {};

template <class F>
struct Unserializable : UnserializableBase {};

template <class F>
struct Serializable : SerializableBase {};

}

#include <object/ObjectBase.h>

namespace zertcore {

typedef u32									error_code;

/**
 * Error
 */
struct Error : public object::ObjectBase<Error>,
		Unserializable<Error>, Serializable<Error>
{
	enum error_type {
		NONE					= 0,
		ERROR					= 1,
	};
	error_type					type;

	error_code					code;
	string						message;

	ZC_TO_STRING("code" << code << "message" << message);

	explicit Error() : type(NONE), code(0) {}
	explicit Error(const string& msg) : type(ERROR), code(0), message(msg) {}
	explicit Error(const error_code& ec, const string& msg) :
			type(NONE), code(ec), message(msg) {type = ec? ERROR: NONE;}

public:
	Error& setError(const string& msg);
	Error& setError(const error_code& c);
	Error& setError(const error_code& c, const string& msg);

	operator bool() const;
	void clear();

public:
	template <class Archiver>
	void serialize(Archiver& archiver) const {
		archiver["code"] & code;
		archiver["msg"] & message;
	}

	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		archiver["code"] & code;
		archiver["msg"] & message;

		return code > 0 || !message.empty();
	}
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

#include "details/errors.hpp"
#include "details/types.hpp"

#endif /* TYPES_H_ */
