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
	typedef Archiver						archiver_type;
	typedef BasicValue						value_type;
	typedef unordered_set<value_type>		value_set_type;

public:
	template <typename T>
	explicit ConditionElement(const key_type& key, const op_type& op,
			const T& v = T(), const T& v1 = T()) :
		op_(op), key_(key), value_(v), value2_(v1) {}
	explicit ConditionElement(const key_type& key, const op_type& op,
			const value_set_type& value_set) :
		op_(op), key_(key), value_set_(value_set) {}
	explicit ConditionElement() : op_(cond::NONE) {}

public:
	bool operator() (archiver_type& ar) const {
		value_type v;
		if (!(ar[key_] & v))
			return false;

		if (op_ == cond::EXISTS)
			return true;

		switch(op_) {
		case cond::EQU:
			return v == value_;
		case cond::GT:
			return v > value_;
		case cond::GTE:
			return v >= value_;
		case cond::LT:
			return v < value_;
		case cond::LTE:
			return v <= value_;
		case cond::MOD:
			return v % value_ == 0;
		case cond::BETWEEN:
			return v >= value_ && v <= value2_;
		case cond::IN:
			return value_set_.find(value_) != value_set_.end();
		default:
			break;
		}

		return false;
	}

public:
	template <class A>
	void serialize(A& archiver) const {
		if (op_ == cond::NONE)
			return ;

		archiver["op"] & op_;
		archiver["key"] & key_;
		archiver["value"] & value_;
		archiver["value2"] & value2_;
		archiver["valset"] & value_set_;
	}

	template <class A>
	bool unserialize(A& archiver) {
		archiver["op"] & op_;
		archiver["key"] & key_;
		archiver["value"] & value_;
		archiver["value2"] & value2_;
		archiver["valset"] & value_set_;

		return op_ > cond::NONE && op_ < cond::MAX;
	}

private:
	op_type						op_;
	key_type					key_;
	value_type					value_,
								value2_;
	value_set_type				value_set_;
};

/**
 * Condition<KeyType, Archiver>
 */
template <typename KeyType, class Archiver>
class Condition :
		public Serializable<Condition<KeyType, Archiver> >,
		public Unserializable<Condition<KeyType, Archiver> >
{
public:
	typedef KeyType							key_type;
	typedef Archiver						archiver_type;

public:
	typedef ConditionElement<key_type, archiver_type>
											element_type;
	typedef vector<element_type>			expressions_type;
	typedef vector<expressions_type>		expression_chain_type;

public:
	Condition() {
		expression_chain_.resize(1);
	}
	Condition(const element_type& elemt) {
		addOr(elemt);
	}

public:
	void addAnd(const element_type& elemt) {
		expression_chain_.back().push_back(elemt);
	}
	void addOr(const element_type& elemt) {
		expression_chain_.resize(expression_chain_.size() + 1);
		expression_chain_.back().push_back(elemt);
	}

public:
	Condition& operator && (const element_type& elemt) {
		addAnd(elemt);
		return *this;
	}
	Condition& operator || (const element_type& elemt) {
		addOr(elemt);
		return *this;
	}
	Condition& operator = (const element_type& elemt) {
		expression_chain_.clear();
		addOr(elemt);
		return *this;
	}

public:
	bool operator() (archiver_type& ar) const {
		if (expression_chain_.empty())
			/**
			 * default its true
			 */
			return true;

		/**
		 * the chain was link with OR
		 * so just one express
		 */
		for (auto it = expression_chain_.begin(); it != expression_chain_.end(); ++it) {
			const expressions_type& chain = *it;
			bool result = true;

			for (auto hit = chain.begin(); hit != chain.end(); ++hit) {
				if (!(*hit)(ar)) {
					result = false;
					break;
				}
			}

			if (result)
				return true;
		}

		return false;
	}

public:
	template <class A>
	void serialize(A& archiver) const {
		archiver["chain"] & expression_chain_;
	}

	template <class A>
	bool unserialize(A& archiver) {
		archiver["chain"] & expression_chain_;
		return true;
	}

private:
	expression_chain_type		expression_chain_;
};

template <typename K, class A>
Condition<K,A> operator || (const ConditionElement<K, A>& a, const ConditionElement<K, A>& b) {
	Condition<K,A> c(a);
	return c || b;
}
template <typename K, class A>
Condition<K,A> operator && (const ConditionElement<K, A>& a, const ConditionElement<K, A>& b) {
	Condition<K,A> c(a);
	return c && b;
}

}}


#endif /* UTILS_CONDITION_CONDITION_H_ */
