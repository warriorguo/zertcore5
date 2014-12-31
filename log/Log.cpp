/*
 * Log.cpp
 *
 *  Created on: 2014Äê11ÔÂ28ÈÕ
 *      Author: Administrator
 */

#include <pch.h>

#include <log/Log.h>

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
    strftime(buf, sizeof(buf), "[%Y-%m-%d.%X]", &tstruct);

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

/**
 * init()
 */
bool Log::init() {
	setIO(STDIN_W);
	return true;
}

void Log::operator << (const __END& v) {
	if (io_handler_) {
		time_.getTime();
		io_handler_(level_, time_, filename_, line_, key_, ss_.str());

		/**
		 * if the log level was final error, throw the error
		 */
		if (level_ == FINAL) {
			throw Error(ss_.str());
		}
	}

	ss_.clear();
}

}}
