/*
 * Archiver.h
 *
 *  Created on: 2015年1月28日
 *      Author: Administrator
 */

#ifndef ZERTCORE_SERIALIZATION_ARCHIVER_H_
#define ZERTCORE_SERIALIZATION_ARCHIVER_H_

#include <pch.h>
#include <utils/types.h>
#include <object/PoolObject.h>

#include "config.h"

namespace zertcore { namespace serialization {
using namespace zertcore::utils;
using namespace zertcore::object;
}}

namespace zertcore { namespace serialization {

/**
 * Seems intrusive_ptr was better choice for the mechanism( could save the children structure )
 * left this for optimization @time(2015.1.28)
 *
 * #children structure just hold the pointer to forbid the data in stream releasing.
 */
/**
 * Archiver<Stream>
 */
template <class Stream>
class Archiver : public PoolObject<Archiver<Stream> >
{
	typedef Archiver<Stream>				self_type;
public:
	typedef Stream							stream_type;
	typedef typename self_type::ptr			ptr;
	typedef vector<ptr>						children_list_type;

public:
	explicit Archiver(const value_type& type) : type_(type) {;}
	virtual ~Archiver() {}

public:
	value_type getType() const {return type_;}

public:
	key_type& key() {return key_;}
	const key_type& key() const {return key_;}

public:
	stream_type& stream() {return stream_;}
	const stream_type& stream() const {return stream_;}

public:
	void addChild(ptr child) {children_list_.push_back(child);}

private:
	key_type					key_;
	value_type					type_;
	stream_type					stream_;

private:
	children_list_type			children_list_;
};

}}


#endif /* ARCHIVER_H_ */
