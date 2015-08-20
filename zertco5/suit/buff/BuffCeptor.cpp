/*
 * BuffCeptor.cpp
 *
 *  Created on: 2015年6月20日
 *      Author: Administrator
 */

#include "Buff.h"

namespace zertcore { namespace suit {

BuffCeptor::BuffCeptor() {
	;
}

void BuffCeptor::
add(const Buff& buff) {
	buff_ptr_map_.insert(buff_ptr_map_type::value_type(&buff, buff.getTick()));
	refresh();

	buff.link(*this);
}

void BuffCeptor::
erase(const Buff& buff) {
	buff_ptr_map_.erase(&buff);
	refresh();

	buff.unlink(*this);
}

void BuffCeptor::
refresh() {
	rv_.value = 0;
	rv_.rate = 0;

	for (auto it = buff_ptr_map_.begin(); it != buff_ptr_map_.end(); ++it) {
		rv_.value += it->first->getValue();
		rv_.rate += it->first->getRate();
	}
}

void BuffCeptor::
updateTick(const ulong& tick) {
	bool change_flag = false;

	for (auto it = buff_ptr_map_.begin(); it != buff_ptr_map_.end(); ) {
		auto hit = it++;

		if (hit->second <= tick) {
			change_flag = true;
			erase(*hit->first);
		}
		else {
			hit->second -= tick;
		}
	}

	if (change_flag)
		refresh();
}

double BuffCeptor::
calc(const double& v) const {
	return v * rv_;
}

}}


