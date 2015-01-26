/*
 * ObjectBase.h
 *
 *  Created on: 2014-10-4
 *      Author: Administrator
 */

#ifndef ZERTCORE_OBJECTBASE_H_
#define ZERTCORE_OBJECTBASE_H_

#include <pch.h>
#include <utils/types.h>

namespace zertcore { namespace object {

/**
 * ObjectBase<Final>
 * Base for every object
 */
template <class Final>
class ObjectBase
{
public:
	const static uint SENTINEL_CODE			= 0xdeadbeaf;

private:
	uint validity_sentinel_;

public:
	explicit ObjectBase() : validity_sentinel_(SENTINEL_CODE) {}
	virtual ~ObjectBase() {
		validity_sentinel_ = 0;
	}

public:
	bool isValid() const {
		return validity_sentinel_ == SENTINEL_CODE;
	}

public:
	/**
	 * every object must declare what it was
	 */
	virtual string toString() const			= 0;
};

#define ZC_TO_STRING(expr)			\
	public: virtual string toString() const {\
		std::stringstream ss; ss << expr; return ss.str();\
	}

}}


#endif /* OBJECTBASE_H_ */
