/*
 * DataProviderManager.h
 *
 *  Created on: 2014Äê11ÔÂ25ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_DATAPROVIDERMANAGER_H_
#define ZERTCORE_DATAPROVIDERMANAGER_H_

#include <pch.h>

#include <utils/types.h>
#include <utils/SingleInstance.h>
#include <db/io/DataProvider.h>

namespace zertcore{ namespace db{ namespace io{
using namespace zertcore::utils;

#ifndef ZT_DATAPROVIDER_KEY_TYPE
typedef u32									key_type;
#else
typedef ZT_DATAPROVIDER_KEY_TYPE			key_type;
#endif

}}}

namespace zertcore{ namespace db{ namespace io{

/**
 * DataProviderManager<Object>
 */
template <typename Object>
class DataProviderManager :
		public SingleInstance<DataProviderManager<Object> >
{
public:
	typedef Object							object_type;
	typedef DataProvider<Object>			data_provider_type;
	typedef typename data_provider_type::ptr
											data_provider_ptr;
public:
	typedef map<key_type, data_provider_ptr>
											data_provider_map_type;

public:
	void reg(const key_type& key, data_provider_ptr dp) {
		data_provider_map_[key] = dp;
	}
	void unreg(const key_type& key) {
		data_provider_map_.erase(key);
	}

public:
	data_provider_ptr get(const key_type& key) {
		typename data_provider_map_type::iterator it = data_provider_map_.find(key);
		if (it == data_provider_map_.end())
			return null;

		return it->second;
	}

private:
	data_provider_map_type		data_provider_map_;
};

}}}


#endif /* DATAPROVIDERMANAGER_H_ */
