/*
 * RPCFetcher.cpp
 *
 *  Created on: 2015年5月4日
 *      Author: Administrator
 */

#include "RPCFetcher.h"
#include <utils/perf/Performance.h>

namespace zertcore { namespace concurrent { namespace rpc {
using namespace zertcore::utils;
}}}

namespace zertcore { namespace concurrent { namespace rpc { namespace details {

template <typename Handler>
static bool calcLessReactTime(Group<RPCClientConnection::ptr>& conns, const uuid_t& id,
		const Handler& handler) {
	RPCClientConnection::ptr ch;
	time_type min;
	min.value = 999999999.;

	conns.foreach([&] (RPCClientConnection::ptr conn) {
		time_type tt = PerformanceManager::Instance().
				getAverageCost("rpc_host:" + conn->getRemoteConfig().toString());
		if (tt < min) {
			min = tt;
			ch = conn;
		}
	});

	if (ch) {
		handler(ch);
		return true;
	}

	return false;
}

template <typename Handler>
static bool calcBaseOnId(Group<RPCClientConnection::ptr>& conns, const uuid_t& id,
		const Handler& handler) {
	if (conns.empty())
		return false;

	RPCClientConnection::ptr conn = conns.at(id % conns.size());
	handler(conn);

	return true;
}

}}}}

namespace zertcore { namespace concurrent { namespace rpc {

bool RPCLessReactTimeFetcher::
fetch(Group<RPCClientConnection::ptr>& conns, const uuid_t& id,
		const each_handler_type& handler) const {
	return details::calcLessReactTime(conns, id, handler);
}

bool RPCAllFetcher::
fetch(Group<RPCClientConnection::ptr>& conns, const uuid_t& id,
			const each_handler_type& handler) const {
	if (conns.empty())
		return false;

	conns.foreach([&] (RPCClientConnection::ptr conn) {
		handler(conn);
	});

	return true;
}

bool RPCIDBaseFetcher::
fetch(Group<RPCClientConnection::ptr>& conns, const uuid_t& id,
			const each_handler_type& handler) const {
	return details::calcBaseOnId(conns, id, handler);
}

bool RPCDefaultFetcher::
fetch(Group<RPCClientConnection::ptr>& conns, const uuid_t& id,
			const each_handler_type& handler) const {
	if (id) {
		return details::calcBaseOnId(conns, id, handler);
	}

	return details::calcLessReactTime(conns, id, handler);
}

}}}
