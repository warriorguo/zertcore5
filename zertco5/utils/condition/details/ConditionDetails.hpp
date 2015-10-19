/*
 * ConditionDetails.hpp
 *
 *  Created on: 2015年10月12日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_CONDITION_DETAILS_CONDITIONDETAILS_HPP_
#define ZERTCORE_UTILS_CONDITION_DETAILS_CONDITIONDETAILS_HPP_

#include "../Condition.h"

namespace zertcore { namespace utils {

template <typename K, class A>
bool ConditionElement<K, A>::
operator() (archiver_type& ar) const {
	value_type v;
	if (!(ar[keys_.keyList()] & v))
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


}}


namespace zertcore { namespace utils {

template <typename K, class A>
bool ConditionGroup<K, A>::
operator() (archiver_type& ar) const {
	/**
	 * default its true
	 */
	bool ret = true;

	/**
	 * the chain was link with OR
	 * so just one express
	 */
	for (auto it = expression_chain_.begin(); it != expression_chain_.end(); ++it) {
		const element_type& elemt = it->v;

		if (it->type == AND) {
			ret = ret && elemt(ar);
		}
		else {
			ret = ret || elemt(ar);
		}

		if (!ret)
			return false;
	}

	for (auto it = condition_chain_.begin(); it != condition_chain_.end(); ++it) {
		const self_type& elemt = it->v;

		if (it->type == AND) {
			ret = ret && elemt(ar);
		}
		else {
			ret = ret || elemt(ar);
		}

		if (!ret)
			return false;
	}

	return true;
}

}}


#endif /* UTILS_CONDITION_DETAILS_CONDITIONDETAILS_HPP_ */
