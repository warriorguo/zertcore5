/*
 * RPCHelper.hpp
 *
 *  Created on: 2015年2月1日
 *      Author: Administrator
 */

#ifndef ZERTCORE_RPCHELPER_HPP_
#define ZERTCORE_RPCHELPER_HPP_

#include "config.h"

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

}}}


#endif /* RPCHELPER_HPP_ */
