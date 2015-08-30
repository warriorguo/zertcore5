/*
 * SerializeValues.hpp
 *
 *  Created on: 2015年6月16日
 *      Author: Administrator
 */

#ifndef ZERTCORE_OBJECT_DETAILS_SERIALIZEVALUES_HPP_
#define ZERTCORE_OBJECT_DETAILS_SERIALIZEVALUES_HPP_

#include <utils/types.h>
#include <serialize/config.h>

namespace zertcore { namespace utils {

/**
 * ValueBase<Archiver>
 */
template <class Archiver>
struct SerializeBase
{
	virtual ~SerializeBase() {}

	virtual void serialize(typename Archiver::serializer_type& ar)
											= 0;
	virtual bool unserialize(const typename Archiver::unserializer_type& ar)
											= 0;
};

/**
 * SerializeValuesHost<Archiver>
 */
template <class Serializer, class Unserializer>
struct SerializeValuesHost
{
public:
	typedef Serializer						serializer_type;
	typedef Unserializer					unserializer_type;
	typedef SerializeBase<SerializeValuesHost<Serializer, Unserializer> >
											value_type;
public:
	void reg(value_type& v) {
		values_list_.push_back(&v);
	}

	void serialize(serializer_type& ar) {
		for (auto it = values_list_.begin(); it != values_list_.end(); ++it) {
			(*it)->serialize(ar);
		}
	}
	bool unserialize(const unserializer_type& ar) {
		bool ret = true;
		for (auto it = values_list_.begin(); it != values_list_.end(); ++it) {
			ret = (*it)->unserialize(ar) && ret;
		}

		return ret;
	}

private:
	vector<value_type *>		values_list_;
};

}}

namespace zertcore { namespace utils { namespace value {
//using namespace ::boost::flyweights;
using namespace zertcore::serialization;

template <typename T, class Archiver1, class Archiver2 = void, class Archiver3 = void>
class ValueWrapper;

template <typename T>
struct KeyWrapper;

}}}

namespace zertcore { namespace utils { namespace value {

template <typename T>
struct KeyWrapper
{
	typedef const T&						type;
	T							key;

	KeyWrapper(const T& k) : key(k) {}

	type get() const {return key;}
};

/**
 * key was in using
 */
template <>
struct KeyWrapper<string>
{
	typedef const string::value_type*		type;
	type						key;

	KeyWrapper(type k) : key(k) {}

	type get() const {return key;}
};

/**
 * Value<T>
 */
template <typename T>
class ValueRef
{
public:
	typedef T&								ref_type;

public:
	ValueRef(typename KeyWrapper<key_type>::type key, ref_type ref) :
		key_(key), v_(ref) {
	}

public:
	value_type& get() {return v_;}
	const value_type& get() const {return v_;}
	void set(const value_type& v) {v_ = v;}

	operator value_type() {return v_;}

protected:
	KeyWrapper<key_type>		key_;
	ref_type					v_;
};

}}}

namespace zertcore { namespace utils { namespace value {

/**
 * ValueWrapper<T, Archiver>
 */
template <typename T, class Archiver1>
class ValueWrapper<T, Archiver1, void, void>:
	public ValueRef<T>,
	public SerializeBase<Archiver1>
{
public:
	typedef typename Archiver1::serializer_type
											serializer1_type;
	typedef typename Archiver1::unserializer_type
											unserializer1_type;

public:
	ValueWrapper(T& v, Archiver1& values1,
			KeyWrapper<key_type>::type key) :
				ValueRef<T>(key, v) {
		values1.template reg(*this);
	}
	virtual ~ValueWrapper() {}

public:
	ValueWrapper& operator= (const T& v) {this->template set(v); return *this;}
	ValueWrapper& operator= (const ValueWrapper& v) {this->template set(v.v_); return *this;}

public:
	virtual void serialize(serializer1_type& ar) {
		ar[this->template key_.get()] & this->template v_;
	}
	virtual bool unserialize(const unserializer1_type& ar) {
		return ar[this->template key_.get()] & this->template v_;
	}
};

/**
 * ValueWrapper<T, Archiver, Archiver>
 */
template <typename T, class Archiver1, class Archiver2>
class ValueWrapper<T, Archiver1, Archiver2, void>:
	public ValueRef<T>,
	public SerializeBase<Archiver1>,
	public SerializeBase<Archiver2>
{
public:
	typedef typename Archiver1::serializer_type
											serializer1_type;
	typedef typename Archiver1::unserializer_type
											unserializer1_type;

	typedef typename Archiver2::serializer_type
											serializer2_type;
	typedef typename Archiver2::unserializer_type
											unserializer2_type;
public:
	ValueWrapper(T& v, Archiver1& values1, Archiver2& values2,
			KeyWrapper<key_type>::type key, const T& def = T()) :
		ValueRef<T>(key, v) {
		values1.template reg(*this);
		values2.template reg(*this);
	}
	virtual ~ValueWrapper() {}

public:
	ValueWrapper& operator= (const T& v) {this->template set(v); return *this;}
	ValueWrapper& operator= (const ValueWrapper& v) {this->template set(v.v_); return *this;}

public:
	virtual void serialize(serializer1_type& ar) {
		ar[this->template key_.get()] & this->template v_;
	}
	virtual bool unserialize(const unserializer1_type& ar) {
		return ar[this->template key_.get()] & this->template v_;
	}
	virtual void serialize(serializer2_type& ar) {
		ar[this->template key_.get()] & this->template v_;
	}
	virtual bool unserialize(const unserializer2_type& ar) {
		return ar[this->template key_.get()] & this->template v_;
	}
};

/**
 * ValueWrapper<T, Archiver, Archiver, Archiver>
 */
template <typename T, class Archiver1, class Archiver2, class Archiver3>
class ValueWrapper:
	public ValueRef<T>,
	public SerializeBase<Archiver1>,
	public SerializeBase<Archiver2>,
	public SerializeBase<Archiver3>
{
public:
	typedef typename Archiver1::serializer_type
											serializer1_type;
	typedef typename Archiver1::unserializer_type
											unserializer1_type;

	typedef typename Archiver2::serializer_type
											serializer2_type;
	typedef typename Archiver2::unserializer_type
											unserializer2_type;

	typedef typename Archiver3::serializer_type
											serializer3_type;
	typedef typename Archiver3::unserializer_type
											unserializer3_type;
public:
	ValueWrapper(T& v, Archiver1& values1,
			Archiver2& values2,
			Archiver3& values3,
			KeyWrapper<key_type>::type key, const T& def = T()) :
		ValueRef<T>(key, v) {
		values1.template reg(*this);
		values2.template reg(*this);
		values3.template reg(*this);
	}
	virtual ~ValueWrapper() {}

public:
	ValueWrapper& operator= (const T& v) {this->template set(v); return *this;}
	ValueWrapper& operator= (const ValueWrapper& v) {this->template set(v.v_); return *this;}

public:
	virtual void serialize(serializer1_type& ar) {
		ar[this->template key_.get()] & this->template v_;
	}
	virtual bool unserialize(const unserializer1_type& ar) {
		return ar[this->template key_.get()] & this->template v_;
	}
	virtual void serialize(serializer2_type& ar) {
		ar[this->template key_.get()] & this->template v_;
	}
	virtual bool unserialize(const unserializer2_type& ar) {
		return ar[this->template key_.get()] & this->template v_;
	}
	virtual void serialize(serializer3_type& ar) {
		ar[this->template key_.get()] & this->template v_;
	}
	virtual bool unserialize(const unserializer3_type& ar) {
		return ar[this->template key_.get()] & this->template v_;
	}
};

}}}

#endif /* OBJECT_DETAILS_SERIALIZEVALUES_HPP_ */
