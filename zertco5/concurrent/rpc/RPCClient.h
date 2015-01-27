/*
 * RPCClient.h
 *
 *  Created on: 2015年1月27日
 *      Author: Administrator
 */

#ifndef ZERTCORE_RPCCLIENT_H_
#define ZERTCORE_RPCCLIENT_H_

#include <pch.h>
#include <net/client/ClientBase.h>

#include "config.h"

namespace zertcore { namespace concurrent { namespace rpc {
using namespace zertcore::net::client;
}}}

namespace zertcore { namespace concurrent { namespace rpc {

/**
 * DataSync Template
 */
template <class T, typename = typename enable_if<is_base_of<Unserializable<T>, T>, T>::type>
struct DataSync
{
	T&							data;

	explicit DataSync(T& target) : data(target) {}
	void operator () (const key_type& key, const oachiver_type& params) {
		data.unserialize(params);
	}
};

/**
 * RPCClient
 */
class RPCClient :
		public ClientBase<RPCClient, RPCConnection>
{
public:
	bool registerDataSyncHandler(const key_type& key, const data_sync_handler_type& handler);

public:
	void call(const key_type& key, const iachiver_type&);
	void call(const key_type& key, const iachiver_type&, const callback_handler_type& handler);

public:

private:
};

}}}


#endif /* RPCCLIENT_H_ */
