/*
 * Serialize.h
 *
 *  Created on: 2014-4-29
 *      Author: Administrator
 */

#ifndef ZERTCORE_SERIALIZE_H_
#define ZERTCORE_SERIALIZE_H_

#include <pch.h>

#include <serialize/config.h>

namespace zertcore { namespace serialization {

template <class F>
struct Serializable
{
	/**
	template <class Archiver>
	friend inline Archiver& operator << (Archiver& archiver, const F& value) {
		const_cast<F &>(value).template serialize(archiver.template object());
		return archiver;
	}
	*/
};


}}

#endif /* SERIALIZE_H_ */
