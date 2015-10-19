/*
 * HttpResponse.h
 *
 *  Created on: 2015年10月16日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_SIMPLEHTTP_HTTPRESPONSE_H_
#define ZERTCORE_UTILS_SIMPLEHTTP_HTTPRESPONSE_H_

#include <pch.h>
#include <utils/types.h>

#include <utils/buffer/SharedBuffer.h>

namespace zertcore { namespace utils {
namespace http {

enum status_type {
	STATUS_NONE								= 0,

	STATUS_OK,
	STATUS_MOVED_PERMANENTLY,

	STATUS_BAD_REQUEST,
	STATUS_UNAUTHORIZED,
	STATUS_FORBIDDDEN,
	STATUS_NOT_FOUND,
	STATUS_NOT_ALLOWED,
	STATUS_NOT_ACCEPTABLE,

	STATUS_SERVER_ERROR,
	STATUS_NOT_IMPLEMENTED,
	STATUS_BAD_GATEWAY,
	STATUS_SERVICE_UNAVAILABLE,
	STATUS_GATEWAY_TIMEOUT,
};

}}}

namespace zertcore { namespace utils {

/**
 * HttpConnection
 */
class HttpResponse
{
public:
	bool response(const http::status_type& status, const SharedBuffer& content);

public:
	const SharedBuffer& output();

private:
	u32							status_;
	SharedBuffer				response_;
};

}}

#endif /* UTILS_SIMPLEHTTP_HTTPRESPONSE_H_ */
