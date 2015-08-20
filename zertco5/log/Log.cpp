/*
 * Log.cpp
 *
 *  Created on: 2014��11��28��
 *      Author: Administrator
 */

#include <pch.h>
#include <thread/Thread.h>

#include "Log.h"

namespace zertcore {

const char* LogLevelNameMap[] = {
	"NONE",
	"NOTE",
	"DEBUG",
	"NOTICE",
	"WARNING",
	"ERROR",
	"FATAL",
};

}

namespace zertcore { namespace log {

/**
 * output to screen
 */
void STDIN_W(const LogDetails& details, const string& log) {
    struct tm tstruct;
    char buf[80];
    time_t ts = (time_t)details.time.value;

    tstruct = *localtime(&ts);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

    switch(details.level) {
    case LogLevel::NOTE: ::printf("\033[3m");break;
    case LogLevel::DEBUG: break;
    case LogLevel::NOTICE: ::printf("\033[1m");break;
    case LogLevel::WARNING: ::printf("\033[32m");break;
    case LogLevel::ERROR: ::printf("\033[33m");break;
    case LogLevel::FATAL: ::printf("\033[31m");break;
    default:break;
    }

	if (details.key) {
		::printf("[%s][%s][%s:%u %s][key:%llu]%s\n", LogLevelNameMap[details.level], buf,
				details.filename.c_str(), details.line, details.func.c_str(), details.key, log.c_str());
	}
	else {
		::printf("[%s][%s][%s:%u %s]%s\n", LogLevelNameMap[details.level], buf,
				details.filename.c_str(), details.line, details.func.c_str(), log.c_str());
	}

	::printf("\033[0m");
}

}}

namespace zertcore { namespace log {
Log::io_handler_type			Log::io_handlers_;
/**
 * init()
 */
bool Log::setup() {
	setIO(&STDIN_W);
	return true;
}

void Log::operator << (const __END& v) {
	string errmsg = move(ss_.str());

	if (!io_handlers_.empty()) {
		data_.time.getTime();
		io_handlers_(data_, errmsg);
	}

	if (data_.level >= ERROR) {
		if (set_error_ptr_) {
			set_error_ptr_->setError(errmsg);
		}

		threadContext().error.setError(errmsg);
	}

	set_error_ptr_ = NULL;

	string throw_msg;
	/**
	 * if the log level was fatal error, throw the error
	 */
	if (data_.level == FATAL) {
		throw_msg = ss_.str();
	}

	ss_.str(string());
	ss_.clear();
	data_.clear();


	if (!throw_msg.empty())
		throw Error(throw_msg);
}

}}
