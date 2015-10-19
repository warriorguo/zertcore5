/*
 * HttpParser.h
 *
 *  Created on: 2015年10月13日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_SIMPLEHTTP_HTTPPARSER_H_
#define ZERTCORE_UTILS_SIMPLEHTTP_HTTPPARSER_H_

#include <pch.h>
#include <utils/types.h>
#include <utils/buffer/SharedBuffer.h>

#ifndef ZC_HTTP_QUERY_KEY_MAX_SIZE
#  define ZC_HTTP_QUERY_KEY_MAX_SIZE		63
#endif

#ifndef ZC_HTTP_QUERY_VALUE_MAX_SIZE
#  define ZC_HTTP_QUERY_VALUE_MAX_SIZE		255
#endif

#ifndef ZC_HTTP_COOKIE_KEY_MAX_SIZE
#  define ZC_HTTP_COOKIE_KEY_MAX_SIZE		63
#endif

#ifndef ZC_HTTP_COOKIE_VALUE_MAX_SIZE
#  define ZC_HTTP_COOKIE_VALUE_MAX_SIZE		255
#endif

namespace zertcore { namespace utils {

enum {
	METHOD_NONE								= 0,
	METHOD_GET								= 1,
	METHOD_POST								= 2,
};

typedef u32									method_type;

/**
 * HttpParser
 */
class HttpParser;

/**
 * HttpHeaderContext
 */
struct HttpHeaderContext
{
	method_type					method{0};
	u32							version{0};

	u32							content_length{0};

	SharedBuffer				uri;
	SharedBuffer				host;
	SharedBuffer				cookie;
	SharedBuffer				user_agent;
	SharedBuffer				accept;
	SharedBuffer				accept_language;
	SharedBuffer				accept_encoding;
	SharedBuffer				referer;
	bool						keep_alive{false};
};

/**
 * HttpContext
 */
struct HttpContext
{
	HttpParser*					parser{nullptr};
	~HttpContext();

	HttpHeaderContext			header;
	SharedBuffer				post;

public:
	const string& getQuery(const string& key);
	const string& getCookie(const string& key);

#ifndef ZC_RELEASE
	void printQuery() {
		::printf("Query:\n");
		for (auto it = query_.begin(); it != query_.end(); ++it) {
			::printf("%s:%s\n", it->first.c_str(), it->second.c_str());
		}
	}
	void printCookie() {
		::printf("Cookie:\n");
		for (auto it = cookie_.begin(); it != cookie_.end(); ++it) {
			::printf("%s:%s\n", it->first.c_str(), it->second.c_str());
		}
	}
#endif

private:
	void parseQuery();
	void parseCookie();

private:
	unordered_map<string, string>
								query_;
	unordered_map<string, string>
								cookie_;
};

/**
 * parseHttp functions
 */
ret_type parseHttp(const SharedBuffer& sb, HttpContext& context);

}}


#endif /* UTILS_SIMPLEHTTP_HTTPPARSER_H_ */
