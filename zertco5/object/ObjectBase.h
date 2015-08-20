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

namespace helper {

struct KeyValueStream
{
	bool						is_key_;

	KeyValueStream() : is_key_(true) {}

	template <typename T>
	KeyValueStream& operator << (const T& t) {
		o_ << (t);

		if (is_key_) {
			o_ << (":");
		}
		else {
			o_ << (",");
		}

		is_key_ = !is_key_;
		return *this;
	}

	string str() const {
		return o_.str();
	}

private:
	std::ostringstream			o_;
};

}

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
	uint						validity_sentinel_;

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

/**
 * expr MUST BE key << value [ << ...] inform
 */
#define ZC_TO_STRING(expr)			\
	public: virtual string toString() const {\
		::zertcore::object::helper::KeyValueStream ss; ss << expr;\
		return ss.str();\
	}

}}


#endif /* OBJECTBASE_H_ */
