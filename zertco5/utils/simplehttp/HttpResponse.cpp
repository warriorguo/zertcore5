/*
 * HttpResponse.cpp
 *
 *  Created on: 2015年10月16日
 *      Author: Administrator
 */

#include "HttpResponse.h"

namespace zertcore { namespace utils {

const char* ResponseCodes[] = {
	NULL,
	"200 OK", // STATUS_OK

	"301 Moved Permanently", // STATUS_MOVED_PERMANENTLY

	"400 Bad Request", // STATUS_BAD_REQUEST
	"401 Unauthorized", // STATUS_UNAUTHORIZED
	"403 Forbidden", // STATUS_FORBIDDDEN
	"404 Not Found", // STATUS_NOT_FOUND
	"405 Method Not Allowed", // STATUS_NOT_FOUND
	"406 Not Acceptable", // STATUS_NOT_ACCEPTABLE

	"500 Internal Server Error", // STATUS_SERVER_ERROR
	"501 Not Implemented", // STATUS_NOT_IMPLEMENTED
	"502 Bad Gateway", // STATUS_BAD_GATEWAY
	"503 Service Unavailable", // STATUS_SERVICE_UNAVAILABLE
	"504 Gateway Timeout", // STATUS_GATEWAY_TIMEOUT
};

#define RESP_SIZE				(sizeof(ResponseCodes) / sizeof(const char*))

}}

namespace zertcore { namespace utils {

bool HttpResponse::
response(const http::status_type& status, const SharedBuffer& content) {
	char buffer[10];

	if ((u32)status >= RESP_SIZE)
		return false;

	sprintf(buffer, "%u", response_.size());

	response_.clear();
	response_.resize(120 /*header max size*/ +
			response_.size()
	);

	response_.add("HTTP/1.1 ");
	response_.add(ResponseCodes[(u32)status]); response_.add("\n");
	response_.add("Content-Length: ");
	response_.add(buffer); response_.add("\nConnection: Close\n\n");

	if (!content.empty())
		response_.add(content);

	return true;
}

const SharedBuffer& HttpResponse::
output() {
	return response_;
}

}}

