/*
 * Condition.h
 *
 *  Created on: 2015年6月27日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_CONDITION_CONDITION_H_
#define ZERTCORE_UTILS_CONDITION_CONDITION_H_

#include <pch.h>
#include <utils/types.h>

#include "BasicValue.h"

namespace zertcore {
namespace cond {

enum {
	NONE									= 0,
	EQU										= 1,
	EXISTS									= 2,
	GT										= 3,
	GTE										= 4,
	LT										= 5,
	LTE										= 6,
	MOD										= 7,
	BETWEEN									= 8,
	IN										= 9,

	MAX,
};

}

}

namespace zertcore { namespace utils {
typedef u32									op_type;

/**
 * ConditionElement<KeyType, Archiver>
 */
template <typename KeyType, class Archiver>
class ConditionElement :
		public Serializable<ConditionElement<KeyType, Archiver> >,
		public Unserializable<ConditionElement<KeyType, Archiver> >
{
public:
	typedef KeyType							key_type;
	typedef details::KeysHolder<KeyType>	keys_holder_type;;
	typedef Archiver						archiver_type;
	typedef BasicValue						value_type;
	typedef unordered_set<value_type>		value_set_type;

public:
	template <typename T>
	explicit ConditionElement(const key_type& key, const op_type& op,
			const T& v = T(), const T& v1 = T()) :
		op_(op), keys_(key), value_(v), value2_(v1) {
		ZC_ASSERT(op != cond::IN);
		// More check here.
	}
	template <typename T>
	explicit ConditionElement(const keys_holder_type& keys, const op_type& op,
			const T& v = T(), const T& v1 = T()) :
		op_(op), keys_(keys), value_(v), value2_(v1) {
		ZC_ASSERT(op != cond::IN);
		// More check here.
	}
	explicit ConditionElement(const key_type& key, const op_type& op,
			const value_set_type& value_set) :
		op_(op), keys_(key), value_set_(value_set) {
		ZC_ASSERT(op == cond::IN);
	}
	explicit ConditionElement(const keys_holder_type& keys, const op_type& op,
			const value_set_type& value_set) :
		op_(op), keys_(keys), value_set_(value_set) {
		ZC_ASSERT(op == cond::IN);
	}
	explicit ConditionElement() : op_(cond::NONE) {}

public:
	bool operator() (archiver_type& ar) const;

public:
	template <class A>
	void serialize(A& archiver) const {
		if (op_ == cond::NONE)
			return ;

		archiver["op"] & op_;
		archiver["keys"] & keys_;
		archiver["value"] & value_;
		archiver["value2"] & value2_;
		archiver["valset"] & value_set_;
	}

	template <class A>
	bool unserialize(A& archiver) {
		archiver["op"] & op_;
		archiver["keys"] & keys_;
		archiver["value"] & value_;
		archiver["value2"] & value2_;
		archiver["valset"] & value_set_;

		return op_ > cond::NONE && op_ < cond::MAX;
	}

private:
	op_type						op_;
	keys_holder_type			keys_;
	value_type					value_,
								value2_;
	value_set_type				value_set_;
};

/**
 * Condition<KeyType, Archiver>
 *
 * assume cond1 was true, cond2 was false
 *
 * cond1 && cond2 -> false
 *
 * cond1 || cond2 -> true
 * cond2 || cond2 -> true
 *
 * cond1 && cond1 || cond2 -> true
 *
 * cond1 && cond2 || cond1 && cond2 was equal (cond1 && cond2) || (cond1 && cond2) -> false
 *
 */
template <typename KeyType, class Archiver>
class ConditionGroup :
		public Serializable<ConditionGroup<KeyType, Archiver> >,
		public Unserializable<ConditionGroup<KeyType, Archiver> >
{
public:
	typedef KeyType							key_type;
	typedef Archiver						archiver_type;

	typedef ConditionGroup<KeyType, Archiver>
											self_type;

public:
	typedef ConditionElement<key_type, archiver_type>
											element_type;

	template <typename T>
	struct Linker : Serializable<Linker<T> >, Unserializable<Linker<T> >
	{
		uint					type;
		T						v;

		template <class A>
		void serialize(A& archiver) const {
			archiver["t"] & type;
			archiver["v"] & v;
		}

		template <class A>
		bool unserialize(A& archiver) {
			archiver["t"] & type;
			archiver["v"] & v;

			return type;
		}
	};

	typedef vector<Linker<element_type> >	expressions_type;
	typedef vector<Linker<self_type> >		condition_chain_type;

public:
	ConditionGroup() {}
	ConditionGroup(const element_type& elemt) {
		addAnd(elemt);
	}

public:
	ConditionGroup& addAnd(const element_type& elemt) {
		Linker<element_type> l;
		l.type = AND;
		l.v = elemt;

		expression_chain_.push_back(l);
		return *this;
	}
	ConditionGroup& addOr(const element_type& elemt) {
		Linker<element_type> l;
		l.type = OR;
		l.v = elemt;

		expression_chain_.push_back(l);
		return *this;
	}
	ConditionGroup& addAnd(const self_type& elemt) {
		Linker<self_type> l;
		l.type = AND;
		l.v = elemt;

		condition_chain_.push_back(l);
		return *this;
	}
	ConditionGroup& addOr(const self_type& elemt) {
		Linker<self_type> l;
		l.type = OR;
		l.v = elemt;

		condition_chain_.push_back(l);
		return *this;
	}

public:
	ConditionGroup& operator && (const element_type& elemt) {
		return addAnd(elemt);
	}
	ConditionGroup& operator || (const element_type& elemt) {
		return addOr(elemt);
	}
	ConditionGroup& operator = (const element_type& elemt) {
		expression_chain_.clear();
		return addAnd(elemt);
	}

	ConditionGroup& operator && (const self_type& elemt) {
		return addAnd(elemt);
	}
	ConditionGroup& operator || (const self_type& elemt) {
		return addOr(elemt);
	}

public:
	bool operator() (archiver_type& ar) const;

public:
	template <class A>
	void serialize(A& archiver) const {
		archiver["expr"] & expression_chain_;
		archiver["cond"] & condition_chain_;
	}

	template <class A>
	bool unserialize(A& archiver) {
		archiver["expr"] & expression_chain_;
		archiver["cond"] & condition_chain_;
		return true;
	}

private:
	expressions_type			expression_chain_;
	condition_chain_type		condition_chain_;
};

template <typename K, class A>
ConditionGroup<K,A> operator || (const ConditionElement<K, A>& a, const ConditionElement<K, A>& b) {
	ConditionGroup<K,A> c(a);
	return c || b;
}
template <typename K, class A>
ConditionGroup<K,A> operator && (const ConditionElement<K, A>& a, const ConditionElement<K, A>& b) {
	ConditionGroup<K,A> c(a);
	return c && b;
}

}}


#endif /* UTILS_CONDITION_CONDITION_H_ */
