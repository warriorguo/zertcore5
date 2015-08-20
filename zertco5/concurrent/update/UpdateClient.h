/*
 * TickerClient.h
 *
 *  Created on: 2015年5月7日
 *      Author: Administrator
 */

#ifndef ZERTCORE_CONCURRENT_UPDATE_UPDATECLIENT_H_
#define ZERTCORE_CONCURRENT_UPDATE_UPDATECLIENT_H_

#include "config.h"

#include <utils/Singleton.h>

namespace zertcore { namespace concurrent { namespace update {
using namespace zertcore::utils;
}}}

namespace zertcore { namespace concurrent { namespace update {

/**
 * UpdateClient
 */
class UpdateClient :
		public Singleton<UpdateClient>
{
public:
	typedef list<updater_type>				updater_list_type;
	typedef updater_list_type::iterator		updater_key_type;

public:
	bool globalInit();
	bool isEnabled() const {return enabled_;}

public:
	updater_key_type registerHandler(const updater_type& handler);
	void unregisterHandler(updater_key_type key);

private:
	void cbUpdateOnce(rpc::key_type key, rpc::oarchiver_type params,
			rpc::iarchiver_type& ret_data);

private:
	list<updater_type>			updater_list_;
	bool						enabled_{false};
};

}}}


#endif /* CONCURRENT_TICK_TICKERCLIENT_H_ */
