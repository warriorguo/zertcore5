/*
 * Serializer_fwd.h
 *
 *  Created on: 2015年1月27日
 *      Author: Administrator
 */

#ifndef ZERTCORE_SERIALIZER_FWD_H_
#define ZERTCORE_SERIALIZER_FWD_H_

#include <pch.h>
#include <utils/types.h>
#include <object/PoolObject.h>

#include "config.h"

namespace zertcore { namespace serialization {
using namespace zertcore::utils;
using namespace zertcore::object;
}}

namespace zertcore {

template <class F>
struct Serializable {};

}

namespace zertcore { namespace serialization {

/**
 * Serializer<Stream>
 */
template <class Stream>
class Serializer : public PoolObject<Serializer<Stream> >, noncopyable
{
	typedef Serializer<Stream>				self_type;
public:
	typedef Stream							stream_type;
	typedef typename self_type::ptr			ptr;

public:
	explicit Serializer();
	explicit Serializer(const value_type& type);

	virtual ~Serializer() {}

public:
	self_type& operator[] (const key_type& key) {
		setKey(key);
		return *this;
	}
	void setKey(const key_type& key);

public:
	template <typename T>
	self_type& operator& (const T& v);

public:
	void setSize(const size_t& size);

public:
	void setValue(const char* v);
	void setValue(ptr v);

	template <typename T>
	void setValue(const T& v);

private:
	key_type					key_;
	value_type					type_;
	stream_type					stream_;
};

}}


#endif /* SERIALIZER_FWD_H_ */
