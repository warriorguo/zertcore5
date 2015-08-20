/*
 * config.h
 *
 *  Created on: 2015年6月13日
 *      Author: Administrator
 */

#ifndef ZERTCORE_NET_UDP_CONFIG_H_
#define ZERTCORE_NET_UDP_CONFIG_H_

#include <pch.h>
#include <utils/types.h>

#include <enet/enet.h>

#include "../config.h"

/**
 * for UDP way to transfer data better not bigger than the MTU payload
 */

namespace zertcore { namespace net { namespace udp {

typedef ENetHost							host_type;
typedef ENetAddress							address_type;
typedef ENetPeer							peer_type;
typedef peer_type*							peer_ptr;


}}}


#endif /* NET_UDP_CONFIG_H_ */
