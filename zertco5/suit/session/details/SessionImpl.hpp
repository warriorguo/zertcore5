/*
 * SessionImpl.hpp
 *
 *  Created on: 2015年7月21日
 *      Author: Administrator
 */

#ifndef ZERTCORE_SUIT_SESSION_SESSIONIMPL_HPP_
#define ZERTCORE_SUIT_SESSION_SESSIONIMPL_HPP_

#include "../Session.h"
#include <core/Runtime.h>

namespace zertcore { namespace suit {

template <u32 S, class D, class C>
typename Session<S, D, C>::on_create_handler_type
								Session<S, D, C>::on_create_handler_;
template <u32 S, class D, class C>
typename Session<S, D, C>::on_sync_handler_type
								Session<S, D, C>::on_sync_handler_;
template <u32 S, class D, class C>
typename Session<S, D, C>::on_data_handler_type
								Session<S, D, C>::on_data_handler_;

template <u32 S, class D, class C>
bool Session<S, D, C>::
init() {
	if (on_create_handler_) {
		on_create_handler_.push(this->template thisPtr());
	}
	return true;
}

template <u32 S, class D, class C>
void Session<S, D, C>::
addConnection(connection_ptr conn) {
	connection_set_.insert(conn);
}

template <u32 S, class D, class C>
void Session<S, D, C>::
eraseConnection(connection_ptr conn) {
	connection_set_.erase(conn);
}

template <u32 S, class D, class C>
void Session<S, D, C>::
send(const SharedBuffer& buff) {
	if (!is_gate_) {
		sed_msg_buffer_.push_back(buff);
		// SessionManager<S, D, C>::Instance().registerSync(this->template thisPtr());
		this->template sync();
	}
	else {
		for (auto it = connection_set_.begin(); it != connection_set_.end(); ++it) {
			(*it)->template write(buff);
		}
	}
}

template <u32 S, class D, class C>
void Session<S, D, C>::
onSync() {
	if (is_gate_) {
		sendBuffers();
	}
	else {
		runCommand();

		if (on_sync_handler_)
			on_sync_handler_.push(this->template thisPtr());
	}
}

template <u32 S, class D, class C>
void Session<S, D, C>::
sendBuffers() {
	for (auto hit = sed_msg_buffer_.begin(); hit != sed_msg_buffer_.end(); ++hit) {
		SharedBuffer& buff = *hit;

		for (auto it = connection_set_.begin(); it != connection_set_.end(); ++it) {
			(*it)->template write(buff);
		}
	}

	sed_msg_buffer_.clear();
}

template <u32 S, class D, class C>
bool Session<S, D, C>::
pushMessage(const SharedBuffer& buff, connection_ptr conn) {
	if (rev_msg_buffer_.size() >= rev_msg_buffer_.capacity())
		return false;

	addConnection(conn);
	rev_msg_buffer_.push_back(buff);

	return true;
}

template <u32 S, class D, class C>
bool Session<S, D, C>::
popMessage(SharedBuffer& buff) {
	if (rev_msg_buffer_.empty()) {
		return false;
	}

	buff = rev_msg_buffer_.front();
	rev_msg_buffer_.pop_front();

	return true;
}

template <u32 S, class D, class C>
bool Session<S, D, C>::
hasMessage() const {
	return !rev_msg_buffer_.empty();
}

}}

#endif /* SUIT_SESSION_SESSIONIMPL_HPP_ */
