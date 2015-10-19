/*
 * test1.cpp
 *
 *  Created on: 2015年10月14日
 *      Author: Administrator
 */

#include <utils/simplehttp/HttpParser.h>
#include <log/Log.h>
#include <details.h>

using namespace zertcore;
using namespace zertcore::utils;

int main() {
	ZCLOG_SETUP();

	SharedBuffer sb, sb2;
	sb.assign("POST /include/httptest.inc.new.php HTTP/1.1\nHost: www.atool.org\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:38.0) Gecko/20100101 Firefox/38.0\nAccept: application/json, text/javascript, */*; q=0.01\nAccept-Language: zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3\nAccept-Encoding: gzip, deflate\nContent-Type: application/x-www-form-urlencoded; charset=UTF-8\nX-Requested-With: XMLHttpRequest\nReferer: http://www.atool.org/httptest.php\nContent-Length: 223\nCookie: PHPSESSID=ch6jf0gfsleueobtchlu41hhi4; Hm_lvt_b169f904ade5ef8ef30b1f93eb03f0ef=1444978828; Hm_lpvt_b169f904ade5ef8ef30b1f93eb03f0ef=1444978828; CNZZDATA5763499=cnzz_eid%3D674376197-1444977118-http%253A%252F%252Fwww.baidu.com%252F%26ntime%3D1444977118; http_url=www.baidu.com; method=POST; params=hahahahaha%3Dhehehehehe%26lalala%3D123%26gogogo%3Dpppppp; headers=\nConnection: keep-alive\nPragma: no-cache\nCache-Control: no-cache\n\nurl=www.baidu.com&params%5B0%5D%5Bname%5D=hahahahaha&params%5B0%5D%5Bvalue%5D=hehehehehe&params%5B1%5D%5Bname%5D=lalala&params%5B1%5D%5Bvalue%5D=123&params%5B2%5D%5Bname%5D=");

	HttpContext hc;
	ret_type r = parseHttp(sb, hc);
	if (r == RET_CONTINUE) {
		sb2.assign("gogogo&params%5B2%5D%5Bvalue%5D=pppppp&method=POST");
		r = parseHttp(sb2, hc);
	}

	::printf("r=%u\n", r);

	::printf("uri:%s\n", (const char *)hc.header.uri.data());
	::printf("method:%u\n", hc.header.method);
	::printf("version:%u\n", hc.header.version);
	::printf("host:%s\n", (const char *)hc.header.host.data());
	::printf("accept:%s\n", (const char *)hc.header.accept.data());
	::printf("accept_encoding:%s\n", (const char *)hc.header.accept_encoding.data());
	::printf("accept_language:%s\n", (const char *)hc.header.accept_language.data());
	::printf("content_length:%u\n", hc.header.content_length);
	::printf("cookie:%s\n", (const char *)hc.header.cookie.data());
	::printf("keep_alive:%s\n", (const char *)hc.header.keep_alive? "YES" : "NO");
	::printf("user_agent:%s\n", (const char *)hc.header.user_agent.data());
	::printf("referer:%s\n\n------------------------------------\n\n", (const char *)hc.header.referer.data());
	::printf("post:%s\n", (const char *)hc.post.data());

	return 0;
}

