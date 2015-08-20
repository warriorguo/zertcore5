/*
 * Buff.cpp
 *
 *  Created on: 2015年6月20日
 *      Author: Administrator
 */

#include "Buff.h"

namespace zertcore { namespace suit {

Buff::Buff() : host_(0), via_(0), tick_(0) {;}
Buff::Buff(const double& r, const double& v, const ulong& tick) : host_(0), via_(0), tick_(tick) {
	rv_.rate = r;
	rv_.value = v;
}

Buff::~Buff() {
	unlinkAll();
}

void Buff::
unlinkAll() const {
	for (auto it = buffceptor_ptr_list_.begin();
			it != buffceptor_ptr_list_.end(); ) {
		auto hit = it++;
		*(*hit) -= *this;
	}
	buffceptor_ptr_list_.clear();
}

}}

