/*
 * Serializer_fwd.h
 *
 *  Created on: 2015年1月27日
 *      Author: Administrator
 */

#ifndef ZERTCORE_SERIALIZER_FWD_H_
#define ZERTCORE_SERIALIZER_FWD_H_

#include "Archiver.h"

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
class Serializer : noncopyable
{
	typedef Serializer<Stream>				self_type;
public:
	typedef Stream							stream_type;
	typedef Archiver<Stream>				archiver_type;
	typedef typename archiver_type::ptr		archiver_ptr;

public:
	explicit Serializer();
	explicit Serializer(const value_type& type);

	virtual ~Serializer() {}

public:
	stream_type& stream() {return archiver_->stream();}
	const stream_type& stream() const {return archiver_->stream();}

public:
	self_type& operator[] (const key_type& key) {
		setKey(key);
		return *this;
	}
	void setKey(const key_type& key);

public:
	template <typename T>
	self_type& operator& (const T& v);

	self_type& operator& (const char *v);

public:
	void setSize(const size_t& size);

public:
	void setValue(const char* v);
	void setValue(self_type& v);

	template <typename T>
	void setValue(const T& v);

private:
	archiver_ptr				archiver_;
};

}}


#endif /* SERIALIZER_FWD_H_ */
