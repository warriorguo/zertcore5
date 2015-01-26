/*
 * config.h
 *
 *  Created on: 2015Äê1ÔÂ7ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_NET_SERVER_CONFIG_H_
#define ZERTCORE_NET_SERVER_CONFIG_H_

#include <pch.h>

#include <utils/types.h>

#include "../config.h"

namespace zertcore { namespace net {

/**
 * ServerConfig
 */
struct ServerConfig :
		public ServiceConfig
{
	string						host;
	u32							port{0};

	u32							accept_nums;
};

}}


#endif /* CONFIG_H_ */
