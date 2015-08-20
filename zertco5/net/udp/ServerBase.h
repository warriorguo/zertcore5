/*
 * ServerBase.h
 *
 *  Created on: 2015年6月13日
 *      Author: Administrator
 */

#ifndef ZERTCORE_NET_UDP_SERVERBASE_H_
#define ZERTCORE_NET_UDP_SERVERBASE_H_

#include "config.h"
#include "../server_config.h"

namespace zertcore { namespace net { namespace udp {

/**
 * ServerBase
 */
template <class Final, class Connection>
class ServerBase
{
private:
	typedef ServerBase<Final, Connection>	self;

public:
	typedef Final							final_type;
	typedef Connection						connection_type;
	typedef typename connection_type::ptr	connection_ptr;
	typedef typename connection_type::pure_ptr
											connection_pure_ptr;
public:
	typedef host_type*						host_ptr;

public:
	typedef unordered_map<connection_pure_ptr, connection_ptr>
											connection_map_type;
public:
	virtual ~ServerBase();

public:
	bool setup(const ServerConfig& rc);

public:
	bool flush();

public:
	void startRun();
	size_t mainThread();

private:
	host_ptr					host_{nullptr};

private:
	connection_map_type			connection_map_;
};

}}}


#endif /* NET_UDP_SERVERBASE_H_ */
