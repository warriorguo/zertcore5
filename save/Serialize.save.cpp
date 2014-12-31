/*
 * Serialize.cpp
 *
 *  Created on: 2014-4-25
 *      Author: Administrator
 */

#include "Serialize.h"

namespace zertcore {

template <typename HostType, typename ValueType,
	typename = typename enable_if<is_base_of<Serializable, ValueType>, ValueType>::type>
bool serialize(HostType& host, const ValueType& value) {
	return value.template serialize(host);
}

template <typename HostType, typename ValueType, class Alloc>
bool serialize(HostType& host, const std::list<ValueType, Alloc>& value) {
	typedef std::list<ValueType, Alloc>		list_type;

	for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		if (!serialize(host, *it)) {
			return false;
		}
	}

	return true;
}

template <typename HostType, typename ValueType, class Alloc>
bool serialize(HostType& host, const std::vector<ValueType, Alloc>& value) {
	typedef std::vector<ValueType, Alloc>	list_type;

	for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		if (!serialize(host, *it)) {
			return false;
		}
	}

	return true;
}

template <typename HostType, typename ValueType, class Alloc>
bool serialize(HostType& host, const std::deque<ValueType, Alloc>& value) {
	typedef std::deque<ValueType, Alloc>	list_type;

	for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		if (!serialize(host, *it)) {
			return false;
		}
	}

	return true;
}

template <typename HostType, typename ValueType, class Compare, class Alloc>
bool serialize(HostType& host, const std::set<ValueType, Compare, Alloc>& value) {
	typedef std::set<ValueType, Compare, Alloc>
											list_type;

	for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		if (!serialize(host, *it)) {
			return false;
		}
	}

	return true;
}

template <typename HostType, typename ValueType, class Compare, class Alloc>
bool serialize(HostType& host, const std::multiset<ValueType, Compare, Alloc>& value) {
	typedef std::multiset<ValueType, Compare, Alloc>
											list_type;

	for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		if (!serialize(host, *it)) {
			return false;
		}
	}

	return true;
}

template <typename HostType, typename T, typename ValueType, class Compare, class Alloc>
bool serialize(HostType& host, const std::map<T, ValueType, Compare, Alloc>& value) {
	typedef std::map<T, ValueType, Compare, Alloc>
											list_type;

	for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		serialization::key_type key = lexical_cast<serialization::key_type>(it->first);
		if (!serialize(host[key], it->second)) {
			return false;
		}
	}

	return true;
}

template <typename HostType, typename T, typename ValueType, class Compare, class Alloc>
bool serialize(HostType& host, const std::multimap<T, ValueType, Compare, Alloc>& value) {
	typedef std::map<T, ValueType, Compare, Alloc>
											list_type;

	for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		serialization::key_type key = lexical_cast<serialization::key_type>(it->first);
		if (!serialize(host[key], it->second)) {
			return false;
		}
	}

	return true;
}

}


/**
 * for unserialize
 */
namespace zertcore {

template <typename HostType, typename ValueType, class Alloc>
bool unserialize(HostType& host, std::list<ValueType, Alloc>& value) {
	typedef std::list<ValueType, Alloc>		list_type;

	for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		if (!unserialize(host, *it)) {
			return false;
		}
	}

	return true;
}

template <typename HostType, typename ValueType, class Alloc>
bool unserialize(HostType& host, std::vector<ValueType, Alloc>& value) {
	typedef std::vector<ValueType, Alloc>	list_type;

	for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		if (!unserialize(host, *it)) {
			return false;
		}
	}

	return true;
}

template <typename HostType, typename ValueType, class Alloc>
bool unserialize(HostType& host, std::deque<ValueType, Alloc>& value) {
	typedef std::deque<ValueType, Alloc>	list_type;

	for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		if (!unserialize(host, *it)) {
			return false;
		}
	}

	return true;
}

template <typename HostType, typename ValueType, class Compare, class Alloc>
bool unserialize(HostType& host, std::set<ValueType, Compare, Alloc>& value) {
	typedef std::set<ValueType, Compare, Alloc>
											list_type;

	for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		if (!unserialize(host, *it)) {
			return false;
		}
	}

	return true;
}

template <typename HostType, typename ValueType, class Compare, class Alloc>
bool unserialize(HostType& host, std::multiset<ValueType, Compare, Alloc>& value) {
	typedef std::multiset<ValueType, Compare, Alloc>
											list_type;

	for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		if (!unserialize(host, *it)) {
			return false;
		}
	}

	return true;
}

template <typename HostType, typename T, typename ValueType, class Compare, class Alloc>
bool unserialize(HostType& host, std::map<T, ValueType, Compare, Alloc>& value) {
	typedef std::map<T, ValueType, Compare, Alloc>
											list_type;

	for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		serialization::key_type key = lexical_cast<serialization::key_type>(it->first);
		if (!unserialize(host[key], it->second)) {
			return false;
		}
	}

	return true;
}

template <typename HostType, typename T, typename ValueType, class Compare, class Alloc>
bool unserialize(HostType& host, std::multimap<T, ValueType, Compare, Alloc>& value) {
	typedef std::map<T, ValueType, Compare, Alloc>
											list_type;

	for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
		serialization::key_type key = lexical_cast<serialization::key_type>(it->first);
		if (!unserialize(host[key], it->second)) {
			return false;
		}
	}

	return true;
}

template <typename HostType, typename ValueType>
bool unserialize(HostType& host, ValueType& value) {
	return unserialize(host, value);
}

template <typename HostType, typename ValueType, typename enable_if<is_base_of<Serializable, ValueType>, int>::type>
bool unserialize(HostType& host, ValueType& value) {
	return value.template unserialize(host);
}

}

namespace zertcore { namespace serialization {


}}

