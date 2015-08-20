/*
 * HttpResponse.cpp
 *
 *  Created on: 2015Äê1ÔÂ11ÈÕ
 *      Author: Administrator
 */

#include "HttpResponse.h"

namespace zertcore { namespace net { namespace details {

HttpResponse::HttpResponse() : status_(STATUS_OK) {
	// TODO Auto-generated constructor stub

}

HttpResponse::~HttpResponse() {
	// TODO Auto-generated destructor stub
}

void HttpResponse::
setStatus(const u32& status, const string& status_msg) {
	;
}

string HttpResponse::
output() {
	std::stringstream ss;

	ss << "HTTP/1.1 " << status_ << " " << "\r\n"
	<< "Content-Length: " << response_content_.size() << "\r\n"
	<< "\r\n"
	<< response_content_;

	return ss.str();
}

} /* namespace details */ } /* namespace net */ } /* namespace zertcore */
