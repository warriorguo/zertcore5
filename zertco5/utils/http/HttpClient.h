/*
 * HttpClient.h
 *
 *  Created on: 2015��1��13��
 *      Author: Administrator
 */

#ifndef ZRETCORE_HTTPCLIENT_H_
#define ZRETCORE_HTTPCLIENT_H_

#include <pch.h>
#include <concurrent/Concurrent.h>

#include <net/tcp/client/ClientBase.h>

#include "HttpConnection.h"

namespace zertcore { namespace concurrent {
using namespace zertcore::concurrent;
}}

namespace zertcore { namespace net { namespace tcp { namespace client{

/**
 * HttpClient
 */
class HttpClient :
		public ClientBase<HttpClient, HttpClientConnection>
{
public:
	typedef ThreadHandler<void (u32 status, const string& content)>
											handler_type;

public:
	virtual ~HttpClient();

public:
	void get(const string& url, const handler_type& handler);
	void post(const string& url, const string& post,
			const handler_type& handler);

private:
};

}}}}

#endif /* HTTPCLIENT_H_ */
