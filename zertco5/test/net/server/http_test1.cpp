/*
 * http_test1.cpp
 *
 *  Created on: 2015Äê1ÔÂ12ÈÕ
 *      Author: Administrator
 */

#include <config/Config.h>
#include <core/Runtime.h>
#include <utils/http/HttpServer.h>
#include <log/Log.h>

using namespace zertcore;
using namespace zertcore::net;

int main() {
	ServerConfig server_config;
	server_config.accept_nums = 10;
	server_config.enable_ssl = false;
	server_config.host = "0.0.0.0";
	server_config.port = 80;
	server_config.thread_nums = 10;

	::printf("define server\n");
	server::HttpServer server;

	config.concurrent.thread_nums = 1;

	::printf("init server..\n");
	ZC_ASSERT(server.init(server_config));

	::printf("RT.init\n");
	RT.init([&]() {

		server::HttpServer::handler_type hd([] (HttpConnection::ptr conn) -> void {
			ZCLOG(NOTICE) << "Hello there!" << End;

			string s = "<html><body><h1>Hello there:"
					+ conn->request().getURI() + "</h1><div>" + conn->request().getPost()
					+ "</div>";

			if (conn->request().getAction() == zertcore::net::details::HttpRequest::ACTION_POST) {
				s += "<div>You are posting to this page, content:" + conn->request().getPost() + "</div>";
			}
			else {
				s += "<form action='/post' method='POST'><input type='text' name='sss'><input type='submit'></form>";
			}
			s += "</body></html>";

			conn->response().setResponse(s);
			conn->flush();
		});

		::printf("set thread index..\n");
		hd.setThreadIndex(0);

		::printf("set handler..\n");
		server.setHandler(hd);
	});
	RT.run();

	return 0;
}
