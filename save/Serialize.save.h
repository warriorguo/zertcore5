/*
 * Serialize.h
 *
 *  Created on: 2014-4-25
 *      Author: Administrator
 */

#ifndef ZERTCORE_SERIALIZE_H_
#define ZERTCORE_SERIALIZE_H_

#include <boost/type_traits.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/utility.hpp>

#include <string>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <deque>

#include <utils/types.h>

namespace zertcore { namespace serialization {

#ifndef ZT_SERIALIZE_KEY_TYPE
typedef std::string							key_type;
#else
typedef ZT_SERIALIZE_KEY_TYPE				key_type;
#endif

}}

namespace zertcore {

using namespace boost;
using namespace zertcore::utils;

struct Serializable{};

/**
 * if the value was derived from Serializer, it should be has
 *
 * template <typename HostType>
 * bool serialize(const HostType&);
 *
 * method!
 */
template <typename HostType, typename ValueType>
bool serialize(HostType& host, const ValueType& value);

template <typename HostType, typename ValueType,
	typename = typename enable_if<is_base_of<Serializable, ValueType>, ValueType>::type>
bool serialize(HostType& host, const ValueType& value);

template <typename HostType>
bool serialize(HostType& host, const char* value);

template <typename HostType>
bool serialize(HostType& host, const char& value);

template <typename HostType>
bool serialize(HostType& host, const unsigned char& value);

template <typename HostType>
bool serialize(HostType& host, const short& value);

template <typename HostType>
bool serialize(HostType& host, const unsigned short& value);

template <typename HostType>
bool serialize(HostType& host, const int& value);

template <typename HostType>
bool serialize(HostType& host, const unsigned int& value);

template <typename HostType>
bool serialize(HostType& host, const long& value);

template <typename HostType>
bool serialize(HostType& host, const unsigned long& value);

template <typename HostType>
bool serialize(HostType& host, const long long& value);

template <typename HostType>
bool serialize(HostType& host, const unsigned long long& value);

template <typename HostType>
bool serialize(HostType& host, const float& value);

template <typename HostType>
bool serialize(HostType& host, const double& value);

template <typename HostType>
bool serialize(HostType& host, const std::string& value);

template <typename HostType, typename ValueType, class Alloc>
bool serialize(HostType& host, const std::list<ValueType, Alloc>& value);

template <typename HostType, typename ValueType, class Alloc>
bool serialize(HostType& host, const std::vector<ValueType, Alloc>& value);

template <typename HostType, typename ValueType, class Alloc>
bool serialize(HostType& host, const std::deque<ValueType, Alloc>& value);

template <typename HostType, typename ValueType, class Compare, class Alloc>
bool serialize(HostType& host, const std::set<ValueType, Compare, Alloc>& value);

template <typename HostType, typename ValueType, class Compare, class Alloc>
bool serialize(HostType& host, const std::multiset<ValueType, Compare, Alloc>& value);

template <typename HostType, typename T, typename ValueType, class Compare, class Alloc>
bool serialize(HostType& host, const std::map<T, ValueType, Compare, Alloc>& value);

template <typename HostType, typename T, typename ValueType, class Compare, class Alloc>
bool serialize(HostType& host, const std::multimap<T, ValueType, Compare, Alloc>& value);

}

namespace zertcore {

using namespace boost;

struct Unserializable{};

/**
 * if the value was derived from Serializer, it should be has
 *
 * template <typename HostType>
 * bool unserialize(HostType&);
 *
 * method!
 */
template <typename HostType, typename ValueType>
bool unserialize(HostType& host, ValueType& value);

template <typename HostType>
bool unserialize(HostType& host, char* value);

template <typename HostType>
bool unserialize(HostType& host, char& value);

template <typename HostType>
bool unserialize(HostType& host, unsigned char& value);

template <typename HostType>
bool unserialize(HostType& host, short& value);

template <typename HostType>
bool unserialize(HostType& host, unsigned short& value);

template <typename HostType>
bool unserialize(HostType& host, int& value);

template <typename HostType>
bool unserialize(HostType& host, unsigned int& value);

template <typename HostType>
bool unserialize(HostType& host, long& value);

template <typename HostType>
bool unserialize(HostType& host, unsigned long& value);

template <typename HostType>
bool unserialize(HostType& host, long long& value);

template <typename HostType>
bool unserialize(HostType& host, unsigned long long& value);

template <typename HostType>
bool unserialize(HostType& host, float& value);

template <typename HostType>
bool unserialize(HostType& host, double& value);

template <typename HostType>
bool unserialize(HostType& host, std::string& value);

template <typename HostType, typename ValueType, class Alloc>
bool unserialize(HostType& host, std::list<ValueType, Alloc>& value);

template <typename HostType, typename ValueType, class Alloc>
bool unserialize(HostType& host, std::vector<ValueType, Alloc>& value);

template <typename HostType, typename ValueType, class Alloc>
bool unserialize(HostType& host, std::deque<ValueType, Alloc>& value);

template <typename HostType, typename ValueType, class Compare, class Alloc>
bool unserialize(HostType& host, std::set<ValueType, Compare, Alloc>& value);

template <typename HostType, typename ValueType, class Compare, class Alloc>
bool unserialize(HostType& host, std::multiset<ValueType, Compare, Alloc>& value);

template <typename HostType, typename T, typename ValueType, class Compare, class Alloc>
bool unserialize(HostType& host, std::map<T, ValueType, Compare, Alloc>& value);

template <typename HostType, typename T, typename ValueType, class Compare, class Alloc>
bool unserialize(HostType& host, std::multimap<T, ValueType, Compare, Alloc>& value);

}

namespace zertcore { namespace serialization {

using namespace boost;

namespace base {
/**
 * _Base for Serializer & Unserializer
 */
template <class Final>
class _Base
{
public:
	typedef Final							final_type;

public:
	virtual ~_Base() {}

public:
	virtual final_type& operator[] (const key_type& key)
											= 0;
};

}

/**
 * Serializer<Final>
 */
template <class Final>
class Serializer :
		public base::_Base<Final>
{
	typedef Serializer						self;

public:
	typedef Final							final_type;

public:
	Serializer() {}
	virtual ~Serializer() {}

public:
	self& operator() (const key_type& key) {key_ = key; return *this;}

public:
	template <typename ValueType>
	void operator& (const ValueType& value) {
		if (isEmpty(key_)) {
			serialize(*static_cast<final_type *>(this), value);
		}
		else {
			serialize((*static_cast<final_type *>(this))[key_], value);
		}

		clearUp(key_);
	}

protected:
	key_type					key_;
};

/**
 * Unserializer<Final>
 */
template <class Final>
class Unserializer :
		public base::_Base<Final>
{
	typedef Unserializer					self;

public:
	typedef Final							final_type;

public:
	Unserializer() {}
	virtual ~Unserializer() {}

public:
	self& operator() (const key_type& key) {key_ = key; return *this;}

public:
	template <typename ValueType>
	void operator& (ValueType& value) {
		if (isEmpty(key_)) {
			unserialize((*this), value);
		}
		else {
			unserialize((*this)[key_], value);
		}

		clearUp(key_);
	}

public:
	/**
	 * if the Unserializer got a container
	 */
	virtual bool begin(final_type&)			= 0;
	/**
	 * move the container to next
	 */
	virtual bool next(final_type&)			= 0;

public:
	virtual key_type key()					= 0;

protected:
	key_type					key_;
};

}} /* namespace zertcore::serialize */

#endif /* SERIALIZE_H_ */
