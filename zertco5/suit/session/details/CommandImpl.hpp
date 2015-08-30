/*
 * CommandImpl.hpp
 *
 *  Created on: 2015年7月27日
 *      Author: Administrator
 */

#ifndef ZERTCORE_SUIT_SESSION_DETAILS_COMMANDIMPL_HPP_
#define ZERTCORE_SUIT_SESSION_DETAILS_COMMANDIMPL_HPP_

#include "../Session.h"

namespace zertcore { namespace suit {

template <u32 S, class D, class C>
bool Session<S, D, C>::
runPrepare() {
	SessionManager<S, D, C>::Instance().registerSync(this->template thisPtr());
	return true;
}

template <u32 S, class D, class C>
bool Session<S, D, C>::
runCommand() {
	SharedBuffer sb;
	if (!popMessage(sb)) {
		return false;
	}

	if (hasMessage()) {
		SessionManager<S, D, C>::Instance().registerSync(this->template thisPtr());
	}

	/**
	cmd::key_type key;
	cmd::params_type params;

	 * parse command

	if (!cmd::parseCommand(sb, key, params)) {
		ZCLOG(ERROR) << "Parse failed" << End;
		return false;
	}
	 */

	/**
	typename cmd::CommandBase<self>::ptr c = cmd::fetchCommand<self>(sb); //cmd::CommandManager<self>::Instance().get(key);
	if (!c) {
		ZCLOG(ERROR) << "Command not found" << End;
		return false;
	}
	c->run(this->template thisPtr(), sb);
	*/

	if (on_data_handler_) {
		on_data_handler_.setParams(this->template thisPtr(), sb);
		on_data_handler_.push();
	}

	return true;
}

}}


#endif /* SUIT_SESSION_DETAILS_COMMANDIMPL_HPP_ */
