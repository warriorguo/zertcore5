/*
 * Archive.h
 *
 *  Created on: 2014-4-29
 *      Author: Administrator
 */

#ifndef ZERTCORE_ARCHIVE_H_
#define ZERTCORE_ARCHIVE_H_

#include <pch.h>

#include <utils/types.h>

#include <serialize/config.h>
#include <serialize/Serialize.h>
#include <serialize/Unserialize.h>

namespace zertcore {namespace serialization {

/**
 * IArchiver<Final>
 */
template <class Final>
class IArchiver
{
	/**
	template <class Archiver, typename T>
	friend Archiver& operator << (Archiver& archiver, const T& v);
	*/
public:
	virtual ~IArchiver() {}

public:
	Final& operator() (const key_type& key) {
		key_ = key;
		return *static_cast<Final *>(this);
	}

	template <typename T>
	Final& operator& (const T& value) {
		return (*static_cast<Final *>(this)) << value;
	}

public:
	const key_type& getKey() const {
		return key_;
	}

public:
	template <typename T>
	Final& operator << (const T& v);

public:
	template <class F>
	Final& operator << (const Serializable<F>& value) {
		const_cast<F &>(value).template serialize(object());
		return *static_cast<Final *>(this);
	}

public:
	template <typename ValueType, class Alloc>
	Final& operator << (const list<ValueType, Alloc>& value) {
		typedef list<ValueType, Alloc>		list_type;

		Final& ar = array();
		for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
			ar << *it;
		}

		return *static_cast<Final *>(this);
	}

	template <typename ValueType, class Alloc>
	Final& operator << (const vector<ValueType, Alloc>& value) {
		typedef vector<ValueType, Alloc>	list_type;

		Final& ar = array();
		for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
			ar << *it;
		}

		return *static_cast<Final *>(this);
	}

	template <typename ValueType, class Alloc>
	Final& operator << (const deque<ValueType, Alloc>& value) {
		typedef deque<ValueType, Alloc>		list_type;

		Final& ar = array();
		for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
			ar << *it;
		}

		return *static_cast<Final *>(this);
	}

	template <typename ValueType, class Compare, class Alloc>
	Final& operator << (const set<ValueType, Compare, Alloc>& value) {
		typedef set<ValueType, Compare, Alloc>
											list_type;

		Final& ar = array();
		for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
			ar << *it;
		}

		return *static_cast<Final *>(this);
	}

	template <typename ValueType, class Compare, class Alloc>
	Final& operator << (const multiset<ValueType, Compare, Alloc>& value) {
		typedef multiset<ValueType, Compare, Alloc>
											list_type;

		Final& ar = array();
		for (typename list_type::const_iterator it = value.begin(); it != value.end(); ++it) {
			ar << *it;
		}

		return *static_cast<Final *>(this);
	}

	template <typename T, typename ValueType, class Compare, class Alloc>
	Final& operator << (const map<T, ValueType, Compare, Alloc>& value) {
		typedef map<T, ValueType, Compare, Alloc>
											map_type;

		Final& ar = object();
		for (typename map_type::const_iterator it = value.begin(); it != value.end(); ++it) {
			key_type key;
			try {
				key = lexical_cast<key_type>(it->first);
			}
			catch(const bad_lexical_cast&) {
				return false;
			}

			ar(key) << it->second;
		}

		return *static_cast<Final *>(this);
	}

	template <typename T, typename ValueType, class Compare, class Alloc>
	Final& operator << (const multimap<T, ValueType, Compare, Alloc>& value) {
		typedef multimap<T, ValueType, Compare, Alloc>
											map_type;

		Final& ar = object();
		for (typename map_type::const_iterator it = value.begin(); it != value.end(); ++it) {
			key_type key;
			try {
				key = lexical_cast<key_type>(it->first);
			}
			catch(const bad_lexical_cast&) {
				return false;
			}

			ar(key) << it->second;
		}

		return *static_cast<Final *>(this);
	}

public:
	virtual Final& array()					= 0;
	virtual Final& object()					= 0;

protected:
	key_type					key_;
};

/**
 * OArchiver<Final>
 */
template <class Final>
class OArchiver
{
	template <class Archiver, typename T>
	friend bool operator >> (Archiver& archiver, T& v);
public:
	OArchiver() {}
	virtual ~OArchiver() {}

public:
	Final& operator() (const key_type& key) {
		key_ = key;
		return *static_cast<Final *>(this);
	}

	template <typename T>
	bool operator& (T& value) {
		return (*static_cast<Final *>(this)) >> value;
	}

public:
	template <typename T>
	bool operator >> (T& v);

public:
	template <class F>
	bool operator >> (const Unserializable<F>& value) {
		return const_cast<F &>(value).
				template unserialize((*static_cast<Final *>(this)));
	}

public:
	template <typename ValueType, class Alloc>
	bool operator >> (list<ValueType, Alloc>& value) {
		value.clear();

		Final ar;
		if (!begin(ar)) {
			return false;
		}

		for (; ar.template isValid(); ar.template next()) {
			ValueType v;
			if (ar >> v)
				value.push_back(v);
		}

		return !value.empty();
	}

	template <typename ValueType, class Alloc>
	bool operator >> (vector<ValueType, Alloc>& value) {
		value.clear();

		Final ar;
		if (!begin(ar)) {
			return false;
		}

		for (; ar.template isValid(); ar.template next()) {
			ValueType v;
			if (ar >> v)
				value.push_back(v);
		}

		return !value.empty();
	}

	template <typename ValueType, class Alloc>
	bool operator >> (deque<ValueType, Alloc>& value) {
		value.clear();

		Final ar;
		if (!begin(ar)) {
			return false;
		}

		for (; ar.template isValid(); ar.template next()) {
			ValueType v;
			if (ar >> v)
				value.push_back(v);
		}

		return !value.empty();
	}

	template <typename ValueType, class Compare, class Alloc>
	bool operator >> (set<ValueType, Compare, Alloc>& value) {
		value.clear();

		Final ar;
		if (!begin(ar)) {
			return false;
		}

		for (; ar.template isValid(); ar.template next()) {
			ValueType v;
			if (ar >> v)
				value.insert(v);
		}

		return !value.empty();
	}

	template <typename ValueType, class Compare, class Alloc>
	bool operator >> (multiset<ValueType, Compare, Alloc>& value) {
		value.clear();

		Final ar;
		if (!begin(ar)) {
			return false;
		}

		for (; ar.template isValid(); ar.template next()) {
			ValueType v;
			if (ar >> v)
				value.insert(v);
		}

		return !value.empty();
	}

	template <typename T, typename ValueType, class Compare, class Alloc>
	bool operator >> (map<T, ValueType, Compare, Alloc>& value) {
		value.clear();

		Final ar;
		if (!begin(ar)) {
			return false;
		}

		for (; ar.template isValid(); ar.template next()) {
			ValueType v;
			if (ar >> v) {
				T key;
				try {
					key = lexical_cast<T>(ar.template key());
				}
				catch(bad_lexical_cast&) {
					continue;
				}

				value[key] = v;
			}
		}

		return !value.empty();
	}

	template <typename T, typename ValueType, class Compare, class Alloc>
	bool operator >> (multimap<T, ValueType, Compare, Alloc>& value) {
		value.clear();

		Final ar;
		if (!begin(ar)) {
			return false;
		}

		for (; ar.template isValid(); ar.template next()) {
			ValueType v;
			if (ar >> v) {
				T key;
				try {
					key = lexical_cast<T>(ar.template key());
				}
				catch(bad_lexical_cast&) {
					continue;
				}

				value[key] = v;
			}
		}

		return !value.empty();
	}

public:
	/**
	 * return the true if the archiver is a container and set the oar to the first element
	 * otherwise return false
	 */
	virtual bool begin(Final& oar)			= 0;
	virtual bool isValid()					= 0;
	virtual bool next()						= 0;

public:
	/**
	 * return current key
	 */
	virtual key_type key()					= 0;

protected:
	key_type					key_;
};

} /* namespace serialization */} /* namespace zertcore */
#endif /* ZERTCORE_ARCHIVE_H_ */
