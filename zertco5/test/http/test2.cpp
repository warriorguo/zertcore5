/*
 * test2.cpp
 *
 *  Created on: 2015年10月17日
 *      Author: Administrator
 */

#include <utils/simplehttp/HttpParser.h>
#include <log/Log.h>
#include <details.h>

using namespace zertcore;
using namespace zertcore::utils;

int main() {
	ZCLOG_SETUP();

	SharedBuffer sb;
	sb.assign("GET /s?wd=hahahahah&rsv_spt=1&rsv_iqid=0xd8e006760008468e&issp=1&f=8&rsv_bp=0&rsv_idx=2&ie=utf-8&tn=baiduhome_pg&rsv_enter=1&rsv_sug3=9&rsv_sug1=7&rsv_t=d827qxhuzWkayyj2%2FUMfElGgk8c%2BUb2Hj82UVXsYrMWMYvoVi8IP5kOpI78wz9LUR3W1&rsv_sug2=0&inputT=1196&rsv_sug4=1196 HTTP/1.1\nHost: www.atool.org\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:38.0) Gecko/20100101 Firefox/38.0\nAccept: application/json, text/javascript, */*; q=0.01\nAccept-Language: zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3\nAccept-Encoding: gzip, deflate\nContent-Type: application/x-www-form-urlencoded; charset=UTF-8\nX-Requested-With: XMLHttpRequest\nReferer: http://www.atool.org/httptest.php\nContent-Length: 223\nCookie: PHPSESSID=ch6jf0gfsleueobtchlu41hhi4; Hm_lvt_b169f904ade5ef8ef30b1f93eb03f0ef=1444978828; Hm_lpvt_b169f904ade5ef8ef30b1f93eb03f0ef=1444978828; CNZZDATA5763499=cnzz_eid%3D674376197-1444977118-http%253A%252F%252Fwww.baidu.com%252F%26ntime%3D1444977118; http_url=www.baidu.com; method=POST; params=hahahahaha%3Dhehehehehe%26lalala%3D123%26gogogo%3Dpppppp; headers=\nConnection: keep-alive\nPragma: no-cache\nCache-Control: no-cache\n\n");

	HttpContext hc;
	ret_type r = parseHttp(sb, hc);

	::printf("r=%u\n", r);
	::printf("uri:%s\n\n", (const char *)hc.header.uri.data());

	string rsv_t = hc.getQuery("rsv_t");

	hc.printQuery();
	return 0;
}
