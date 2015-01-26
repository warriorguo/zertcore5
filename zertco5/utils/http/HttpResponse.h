/*
 * HttpResponse.h
 *
 *  Created on: 2015Äê1ÔÂ11ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_HTTPRESPONSE_H_
#define ZERTCORE_HTTPRESPONSE_H_

#include <pch.h>
#include <utils/types.h>

namespace zertcore { namespace net { namespace details {

/**
 * HttpConnection
 */
class HttpResponse
{
public:
	enum {
		STATUS_NONE							= 0,

		STATUS_OK							= 200,

		STATUS_MOVED_PERMANENTLY			= 301,

		STATUS_BAD_REQUEST					= 400,
		STATUS_UNAUTHORIZED					= 401,
		STATUS_FORBIDDDEN					= 403,
		STATUS_NOT_FOUND					= 404,
		STATUS_NOT_ACCEPTABLE				= 406,

		STATUS_SERVER_ERROR					= 500,
		STATUS_BAD_GATEWAY					= 502,
		STATUS_SERVICE_UNAVAILABLE			= 503,
		STATUS_GATEWAY_TIMEOUT				= 504,
	};

public:
	HttpResponse();
	virtual ~HttpResponse();

public:
	void setStatus(const u32& status, const string& status_msg = string());
	void setResponse(const string& content) {response_content_ = content;}

public:
	string output();

private:
	u32							status_;
	string						response_content_;
};

} /* namespace details */ } /* namespace net */ } /* namespace zertcore */

#endif /* HTTPRESPONSE_H_ */
