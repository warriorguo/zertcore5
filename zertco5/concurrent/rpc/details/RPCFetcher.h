/*
 * RPCFetcher.h
 *
 *  Created on: 2015年5月4日
 *      Author: Administrator
 */

#ifndef ZERTCORE_CONCURRENT_RPC_DETAILS_RPCFETCHER_H_
#define ZERTCORE_CONCURRENT_RPC_DETAILS_RPCFETCHER_H_

#include "../config.h"
#include "../RPCConnection.h"

#include <utils/group/Group.h>

namespace zertcore { namespace concurrent { namespace rpc {

/**
 * RPCCallFetcher
 */
struct RPCCallFetcher
{
	typedef function<void (RPCClientConnection::ptr conn)>
											each_handler_type;
	virtual ~RPCCallFetcher() {}

	virtual bool fetch(Group<RPCClientConnection::ptr>& conns, const uuid_t& id,
			const each_handler_type& handler) const
											= 0;
};

/**
 * RPCLessReactTimeFetcher,
 * fetch one which the less host
 */
struct RPCLessReactTimeFetcher : public RPCCallFetcher
{
	virtual bool fetch(Group<RPCClientConnection::ptr>& conns, const uuid_t& id,
			const each_handler_type& handler) const;
};

/**
 * RPCAllFetcher
 * it would fetch all the conn one by one
 */
struct RPCAllFetcher : public RPCCallFetcher
{
	virtual bool fetch(Group<RPCClientConnection::ptr>& conns, const uuid_t& id,
			const each_handler_type& handler) const;
};

/**
 * RPCIDBaseFetcher
 */
struct RPCIDBaseFetcher : public RPCCallFetcher
{
	virtual bool fetch(Group<RPCClientConnection::ptr>& conns, const uuid_t& id,
			const each_handler_type& handler) const;
};

/**
 * RPCIDBaseFetcher
 */
struct RPCDefaultFetcher : public RPCCallFetcher
{
	virtual bool fetch(Group<RPCClientConnection::ptr>& conns, const uuid_t& id,
			const each_handler_type& handler) const;
};

}}}


#endif /* CONCURRENT_RPC_DETAILS_RPCFETCHER_H_ */
