/*
 * Log.cpp
 *
 *  Created on: 2014��11��28��
 *      Author: Administrator
 */

#include <pch.h>
#include "Log.h"

namespace zertcore {

const char* LogLevelNameMap[] = {
	"NONE",
	"NOTE",
	"DEBUG",
	"NOTICE",
	"ERROR",
	"FINAL",
};

}

namespace zertcore { namespace log {

/**
 * output to screen
 */
void STDIN_W(const LogLevel& level, const time_type& now,
			const string& filename, const uint& line, const uuid_t& key, const string& log) {
    struct tm tstruct;
    char buf[80];
    time_t ts = (time_t)now.value;

    tstruct = *localtime(&ts);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

	if (key) {
		::printf("[%s][%s][%s:%u][key:%llu]%s\n", LogLevelNameMap[level], buf,
				filename.c_str(), line, key, log.c_str());
	}
	else {
		::printf("[%s][%s][%s:%u]%s\n", LogLevelNameMap[level], buf,
				filename.c_str(), line, log.c_str());
	}
}

}}

namespace zertcore { namespace log {
Log::io_handler_type			Log::io_handler_;
/**
 * init()
 */
bool Log::init() {
	setIO(&STDIN_W);
	return true;
}

}}
