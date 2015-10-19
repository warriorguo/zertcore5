/*
 * Tags.h
 *
 *  Created on: 2015-2-17
 *      Author: Administrator
 */

#ifndef ZERTCORE_TAGSMAP_TAGS_H_
#define ZERTCORE_TAGSMAP_TAGS_H_

#include <pch.h>
#include <utils/types.h>
#include <object/PoolObject.h>

namespace zertcore { namespace utils { namespace tags {
using namespace zertcore::object;

struct Unique {};
}}}

namespace zertcore { namespace utils { namespace tags {

template <typename Type, typename Node, typename T>
struct TagTraits;

template <typename Node, typename T>
struct TagTraits<NORMAL, Node, T>
{
	typedef unordered_multimap<T, Node>		map_type;
};

template <typename Node, typename T>
struct TagTraits<UNIQUE, Node, T>
{
	typedef unordered_map<T, Node>			map_type;
};

/**
 * Tags
 */
template <typename Type, typename Value, typename T1, typename T2, typename T3, typename T4, typename T5>
struct Tags;

template <typename Type, typename Value, typename T1, typename T2, typename T3, typename T4, typename T5>
struct Tags
{
	typedef Value							value_type;

	struct Node;
	typedef typename ObjectTraits<Node>::ptr
											node_ptr;

	typedef typename TagTraits<Type, T1, node_ptr>::map_type
											t1_map_type;
	typedef typename TagTraits<Type, T2, node_ptr>::map_type
											t2_map_type;
	typedef typename TagTraits<Type, T3, node_ptr>::map_type
											t3_map_type;
	typedef typename TagTraits<Type, T4, node_ptr>::map_type
											t4_map_type;
	typedef typename TagTraits<Type, T5, node_ptr>::map_type
											t5_map_type;

	t1_map_type					tags1;
	t2_map_type					tags2;
	t3_map_type					tags3;
	t4_map_type					tags4;
	t5_map_type					tags5;

	struct Node : public PoolObject<Node>
	{
		friend class Tags<Type, Value, T1, T2, T3, T4, T5>;
		value_type				value;

	private:
		vector<typename t1_map_type::iterator>
								its1;
		vector<typename t2_map_type::iterator>
								its2;
		vector<typename t3_map_type::iterator>
								its3;
		vector<typename t4_map_type::iterator>
								its4;
		vector<typename t5_map_type::iterator>
								its5;
	};

	void erase(node_ptr n) {
		for (u32 i = 0; i < n->its1.size(); ++i) {
			tags1.erase(n->its1[i]);
		}
		for (u32 i = 0; i < n->its2.size(); ++i) {
			tags2.erase(n->its2[i]);
		}
		for (u32 i = 0; i < n->its3.size(); ++i) {
			tags3.erase(n->its3[i]);
		}
		for (u32 i = 0; i < n->its4.size(); ++i) {
			tags4.erase(n->its4[i]);
		}
		for (u32 i = 0; i < n->its5.size(); ++i) {
			tags5.erase(n->its5[i]);
		}
	}

	void insert(node_ptr n, const T1& key) {
		n->its1.push_back(tags1.insert(key));
	}
	void insert(node_ptr n, const T2& key) {
		n->its2.push_back(tags2.insert(key));
	}
	void insert(node_ptr n, const T3& key) {
		n->its3.push_back(tags3.insert(key));
	}
	void insert(node_ptr n, const T4& key) {
		n->its4.push_back(tags4.insert(key));
	}
	void insert(node_ptr n, const T5& key) {
		n->its5.push_back(tags5.insert(key));
	}

	bool find(const T1& key,
			pair<typename t1_map_type::iterator,
			typename t1_map_type::iterator>& p) const {
		p = tags1.find(key);
		if (p.first == p.second) {
			return false;
		}

		return true;
	}
	bool find(const T2& key,
			pair<typename t2_map_type::iterator,
			typename t2_map_type::iterator>& p) const {
		p = tags2.find(key);
		if (p.first == p.second) {
			return false;
		}

		return true;
	}
	bool find(const T3& key,
			pair<typename t3_map_type::iterator,
			typename t3_map_type::iterator>& p) const {
		p = tags3.find(key);
		if (p.first == p.second) {
			return false;
		}

		return true;
	}
	bool find(const T4& key,
			pair<typename t4_map_type::iterator,
			typename t4_map_type::iterator>& p) const {
		p = tags4.find(key);
		if (p.first == p.second) {
			return false;
		}

		return true;
	}
	bool find(const T5& key,
			pair<typename t5_map_type::iterator,
			typename t5_map_type::iterator>& p) const {
		p = tags5.find(key);
		if (p.first == p.second) {
			return false;
		}

		return true;
	}
};

template <typename Type, typename Value, typename T1, typename T2, typename T3, typename T4>
struct Tags<Type, Value, T1, T2, T3, T4, void>
{
	typedef Value							value_type;

	struct Node;
	typedef typename ObjectTraits<Node>::ptr
											node_ptr;

	typedef typename TagTraits<Type, T1, node_ptr>::map_type
											t1_map_type;
	typedef typename TagTraits<Type, T2, node_ptr>::map_type
											t2_map_type;
	typedef typename TagTraits<Type, T3, node_ptr>::map_type
											t3_map_type;
	typedef typename TagTraits<Type, T4, node_ptr>::map_type
											t4_map_type;

	t1_map_type					tags1;
	t2_map_type					tags2;
	t3_map_type					tags3;
	t4_map_type					tags4;

	struct Node : public PoolObject<Node>
	{
		friend class Tags<Type, Value, T1, T2, T3, T4, void>;

		value_type				value;

	private:
		vector<typename t1_map_type::iterator>
								its1;
		vector<typename t2_map_type::iterator>
								its2;
		vector<typename t3_map_type::iterator>
								its3;
		vector<typename t4_map_type::iterator>
								its4;
	};

	void erase(node_ptr n) {
		for (u32 i = 0; i < n->its1.size(); ++i) {
			tags1.erase(n->its1[i]);
		}
		for (u32 i = 0; i < n->its2.size(); ++i) {
			tags2.erase(n->its2[i]);
		}
		for (u32 i = 0; i < n->its3.size(); ++i) {
			tags3.erase(n->its3[i]);
		}
		for (u32 i = 0; i < n->its4.size(); ++i) {
			tags4.erase(n->its4[i]);
		}
	}

	void insert(node_ptr n, const T1& key) {
		n->its1.push_back(tags1.insert(key));
	}
	void insert(node_ptr n, const T2& key) {
		n->its2.push_back(tags2.insert(key));
	}
	void insert(node_ptr n, const T3& key) {
		n->its3.push_back(tags3.insert(key));
	}
	void insert(node_ptr n, const T4& key) {
		n->its4.push_back(tags4.insert(key));
	}

	bool find(const T1& key,
			pair<typename t1_map_type::iterator,
			typename t1_map_type::iterator>& p) const {
		p = tags1.find(key);
		if (p.first == p.second) {
			return false;
		}

		return true;
	}
	bool find(const T2& key,
			pair<typename t2_map_type::iterator,
			typename t2_map_type::iterator>& p) const {
		p = tags2.find(key);
		if (p.first == p.second) {
			return false;
		}

		return true;
	}
	bool find(const T3& key,
			pair<typename t3_map_type::iterator,
			typename t3_map_type::iterator>& p) const {
		p = tags3.find(key);
		if (p.first == p.second) {
			return false;
		}

		return true;
	}
	bool find(const T4& key,
			pair<typename t4_map_type::iterator,
			typename t4_map_type::iterator>& p) const {
		p = tags4.find(key);
		if (p.first == p.second) {
			return false;
		}

		return true;
	}
};

template <typename Type, typename Value, typename T1, typename T2, typename T3>
struct Tags<Type, Value, T1, T2, T3, void, void>
{
	typedef Value							value_type;

	struct Node;
	typedef typename ObjectTraits<Node>::ptr
											node_ptr;

	typedef typename TagTraits<Type, T1, node_ptr>::map_type
											t1_map_type;
	typedef typename TagTraits<Type, T2, node_ptr>::map_type
											t2_map_type;
	typedef typename TagTraits<Type, T3, node_ptr>::map_type
											t3_map_type;

	t1_map_type					tags1;
	t2_map_type					tags2;
	t3_map_type					tags3;

	struct Node : public PoolObject<Node>
	{
		friend class Tags<Type, Value, T1, T2, T3, void, void>;
		value_type				value;

	private:
		vector<typename t1_map_type::iterator>
								its1;
		vector<typename t2_map_type::iterator>
								its2;
		vector<typename t3_map_type::iterator>
								its3;
	};

	void erase(node_ptr n) {
		for (u32 i = 0; i < n->its1.size(); ++i) {
			tags1.erase(n->its1[i]);
		}
		for (u32 i = 0; i < n->its2.size(); ++i) {
			tags2.erase(n->its2[i]);
		}
		for (u32 i = 0; i < n->its3.size(); ++i) {
			tags3.erase(n->its3[i]);
		}
	}

	void insert(node_ptr n, const T1& key) {
		n->its1.push_back(tags1.insert(key));
	}
	void insert(node_ptr n, const T2& key) {
		n->its2.push_back(tags2.insert(key));
	}
	void insert(node_ptr n, const T3& key) {
		n->its3.push_back(tags3.insert(key));
	}

	bool find(const T1& key,
			pair<typename t1_map_type::iterator,
			typename t1_map_type::iterator>& p) const {
		p = tags1.find(key);
		if (p.first == p.second) {
			return false;
		}

		return true;
	}
	bool find(const T2& key,
			pair<typename t2_map_type::iterator,
			typename t2_map_type::iterator>& p) const {
		p = tags2.find(key);
		if (p.first == p.second) {
			return false;
		}

		return true;
	}
	bool find(const T3& key,
			pair<typename t3_map_type::iterator,
			typename t3_map_type::iterator>& p) const {
		p = tags3.find(key);
		if (p.first == p.second) {
			return false;
		}

		return true;
	}
};

template <typename Type, typename Value, typename T1, typename T2>
struct Tags<Type, Value, T1, T2, void, void, void>
{
	typedef Value							value_type;

	struct Node;
	typedef typename ObjectTraits<Node>::ptr
											node_ptr;

	typedef typename TagTraits<Type, T1, node_ptr>::map_type
											t1_map_type;
	typedef typename TagTraits<Type, T2, node_ptr>::map_type
											t2_map_type;

	t1_map_type					tags1;
	t2_map_type					tags2;

	struct Node : public PoolObject<Node>
	{
		friend class Tags<Type, Value, T1, T2, void, void, void>;
		value_type				value;

	private:
		vector<typename t1_map_type::iterator>
								its1;
		vector<typename t2_map_type::iterator>
								its2;
	};

	void erase(node_ptr n) {
		for (u32 i = 0; i < n->its1.size(); ++i) {
			tags1.erase(n->its1[i]);
		}
		for (u32 i = 0; i < n->its2.size(); ++i) {
			tags2.erase(n->its2[i]);
		}
	}

	void insert(node_ptr n, const T1& key) {
		n->its1.push_back(tags1.insert(key));
	}
	void insert(node_ptr n, const T2& key) {
		n->its2.push_back(tags2.insert(key));
	}

	bool find(const T1& key,
			pair<typename t1_map_type::iterator,
			typename t1_map_type::iterator>& p) const {
		p = tags1.find(key);
		if (p.first == p.second) {
			return false;
		}

		return true;
	}
	bool find(const T2& key,
			pair<typename t2_map_type::iterator,
			typename t2_map_type::iterator>& p) const {
		p = tags2.find(key);
		if (p.first == p.second) {
			return false;
		}

		return true;
	}
};

template <typename Type, typename Value, typename T1>
struct Tags<Type, Value, T1, void, void, void, void>
{
	typedef Value							value_type;

	struct Node;
	typedef typename ObjectTraits<Node>::ptr
											node_ptr;

	typedef typename TagTraits<Type, T1, node_ptr>::map_type
											t1_map_type;

	t1_map_type					tags1;

	struct Node : public PoolObject<Node>
	{
		friend class Tags<Type, Value, T1, void, void, void, void>;
		value_type				value;

	private:
		vector<typename t1_map_type::iterator>
								its1;
	};

	void erase(node_ptr n) {
		for (u32 i = 0; i < n->its1.size(); ++i) {
			tags1.erase(n->its1[i]);
		}
	}

	void insert(node_ptr n, const T1& key) {
		n->its1.push_back(tags1.insert(key));
	}

	bool find(const T1& key,
			pair<typename t1_map_type::iterator,
			typename t1_map_type::iterator>& p) const {
		p = tags1.find(key);
		if (p.first == p.second) {
			return false;
		}

		return true;
	}
};

template <typename Value>
struct getIterator
{
	template <typename T>
	Value operator() (T& it) {
		return it->second->value;
	}
};

}}}


#endif /* TAGS_H_ */
