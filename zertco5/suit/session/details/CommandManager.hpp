/*
 * CommandManager.cpp
 *
 *  Created on: 2015年7月27日
 *      Author: Administrator
 */

#include "../CommandBase.h"

namespace zertcore { namespace suit { namespace cmd {

template <class Session>
bool CommandManager<Session>::
reg(const key_type& key, typename CommandBase<Session>::ptr c) {
	cmd_map_[key] = c;
	return true;
}

template <class Session>
typename CommandBase<Session>::ptr CommandManager<Session>::
get(const key_type& key) {
	auto it = cmd_map_.find(key);
	if (it == cmd_map_.end())
		return nullptr;

	return it->second;
}

}}}

