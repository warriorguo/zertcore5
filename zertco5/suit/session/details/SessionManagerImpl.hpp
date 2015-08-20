/*
 * SessionManagerImpl.hpp
 *
 *  Created on: 2015年8月17日
 *      Author: Administrator
 */

#ifndef ZERTCORE_SUIT_SESSION_DETAILS_SESSIONMANAGERIMPL_HPP_
#define ZERTCORE_SUIT_SESSION_DETAILS_SESSIONMANAGERIMPL_HPP_

#include "../Session.h"
#include <concurrent/update/UpdateClient.h>

namespace zertcore { namespace suit {
using namespace zertcore::concurrent;
}}

namespace zertcore { namespace suit {

template <u32 S, class D, class C>
typename SessionManager<S, D, C>::session_ptr SessionManager<S, D, C>::
createSession() {
	return this->template create(++id_counter_, 0);
}

template <u32 S, class D, class C>
bool SessionManager<S, D, C>::
setup() {
	if (update::UpdateClient::Instance().isEnabled()) {
		update::UpdateClient::Instance().registerHandler([this] (u32 count) {
			handleSessions();
		});
	}
	else {
		RT.addUpdater(&SessionManager<S, D, C>::handleSessions, this);
	}

	is_setup_ = true;
	return true;
}

template <u32 S, class D, class C>
void SessionManager<S, D, C>::
registerSync(session_ptr session) {
	ZC_ASSERT(is_setup_);
	sync_list_.add(session);
}

template <u32 S, class D, class C>
u32 SessionManager<S, D, C>::
handleSessions() {
	u32 ret = sync_list_.size();
	sync_list_.foreach([] (session_ptr session) {
		session->runCommand();
	});

	return ret;
}

}}


#endif /* SUIT_SESSION_DETAILS_SESSIONMANAGERIMPL_HPP_ */
