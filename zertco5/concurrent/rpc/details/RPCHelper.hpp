/*
 * RPCHelper.hpp
 *
 *  Created on: 2015年2月1日
 *      Author: Administrator
 */

#ifndef ZERTCORE_RPCHELPER_HPP_
#define ZERTCORE_RPCHELPER_HPP_

#include "../config.h"
#include "../RPC.h"

namespace zertcore { namespace concurrent { namespace rpc {

/**
 * DataSync Template
 */
template <class T, typename = typename BinarySwitch<is_base_of<Unserializable<T>, T>::value, T, void>::type>
struct DataSync
{
	T&							data;

	explicit DataSync(T& target) : data(target) {;}
	void operator () (key_type key, oarchiver_type params) {
		data.unserialize(params);
	}
};

template <class T>
inline bool static registerDataSync(const key_type& key, T& target) {
	DataSync<T> handler(target);
	return RPC.registerDataSyncHandler(key, bind<void>(handler, _1, _2));
}

}}}


#endif /* RPCHELPER_HPP_ */
