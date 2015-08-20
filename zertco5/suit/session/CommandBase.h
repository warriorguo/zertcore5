/*
 * CommandBase.h
 *
 *  Created on: 2015年7月27日
 *      Author: Administrator
 */

#ifndef ZERTCORE_SUIT_SESSION_COMMANDBASE_H_
#define ZERTCORE_SUIT_SESSION_COMMANDBASE_H_

#include <pch.h>
#include <utils/types.h>
#include <utils/Singleton.h>
#include <utils/buffer/SharedBuffer.h>

#include <core/Runtime.h>

namespace zertcore { namespace suit { namespace cmd {
using namespace zertcore::utils;

#ifdef ZC_CMD_KEY
typedef ZC_CMD_KEY							key_type;
#else
typedef string								key_type;
#endif

#ifdef ZC_CMD_PARAMS
typedef ZC_CMD_PARAMS						params_type;
#else
}}}

#include <concurrent/rpc/config.h>

namespace zertcore { namespace suit { namespace cmd {
typedef concurrent::rpc::oarchiver_type		params_type;
#endif

bool parseCommand(const SharedBuffer&, key_type& key, params_type& params);

}}}

namespace zertcore { namespace suit { namespace cmd {

/**
 * CommandBase
 */
template <class Session>
class CommandBase
{
public:
	typedef Session							session_type;
	typedef typename Session::ptr			session_ptr;

public:
	typedef CommandBase*					ptr;

public:
	virtual ~CommandBase() {}

public:
	/**
	 * running in the game main thread
	 */
	virtual void run(session_ptr session, const key_type& key, const params_type& params)
											= 0;

private:
};

}}}

namespace zertcore { namespace suit { namespace cmd {
/**
 * CommandManager
 */
template <class Session>
class CommandManager :
		public Singleton<CommandManager<Session> >
{
public:
	typedef typename CommandBase<Session>::ptr
											command_ptr;
	typedef unordered_map<key_type, command_ptr>
											cmd_map_type;

public:
	bool reg(const key_type& key, command_ptr);
	command_ptr get(const key_type& key);

private:
	cmd_map_type				cmd_map_;
};

}}}

namespace zertcore { namespace suit { namespace cmd {

/**
 * Register Command
 */
template <class Session>
inline static bool reg(const key_type& key, typename CommandBase<Session>::ptr c) {
	return CommandManager<Session>::Instance().reg(key, c);
}

}}}


#endif /* SUIT_SESSION_COMMANDBASE_H_ */
