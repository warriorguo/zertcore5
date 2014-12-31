/*
 * Log.h
 *
 *  Created on: 2014Äê11ÔÂ19ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_LOG_H_
#define ZERTCORE_LOG_H_

#include <pch.h>

#include <utils/types.h>
#include <utils/time/TimeType.h>
#include <utils/Singleton.h>

#include <object/ObjectBase.h>

namespace zertcore { namespace log {
using namespace zertcore::utils;
using namespace zertcore::object;
}}

namespace zertcore {

/**
 * Log level
 */
enum LogLevel
{
	NONE									= 0,
	NOTE									= 1,
	DEBUG									= NOTE,
	NOTICE									= NOTE + 1,
	ERROR									= NOTICE + 1,
	FINAL									= ERROR + 1,

	LEVEL_MAX								= FINAL,
};

static const char* LogLevelNameMap[] = {
	"NONE",
	"NOTE",
	"DEBUG",
	"NOTICE",
	"ERROR",
	"FINAL",
};

}

namespace zertcore { namespace log {

void STDIN_W(const LogLevel& level, const time_type& time,
			const string& filename, const uint& line, const uuid_t& key, const string& log);

/**
 * Log,
 * FIXME: support for multi-thread
 */
class Log :
		public Singleton<Log>
{
public:
	typedef function<void (const LogLevel& level, const time_type& time,
			const string& filename, const uint& line, const uuid_t& key, const string& log)>
											io_handler_type;

public:
	Log() : level_(NONE), key_(0), line_(0) {}

public:
	bool init();

public:
	void setIO(io_handler_type handler) {
		io_handler_ = handler;
	}

public:
	template <typename T>
	Log& operator << (const T& v) {
		_logText(v, is_arithmetic<T>());
		return *this;
	}
	template <typename T>
	Log& operator << (const T* v) {
		return operator << (*v);
	}
	template <typename T>
	Log& operator << (const SMART_PTR(T)& v) {
		return operator << (*v);
	}

public:
	/**
	 * the end to this record of log
	 */
	void operator << (const __END& v);

public:
	Log& setLevel(const LogLevel& level) {
		level_ = level;
		return *this;
	}
	Log& setKey(const uuid_t& key) {
		key_ = key;
		return *this;
	}
	Log& setFilename(const string& filename) {
		filename_ = filename;
		return *this;
	}
	Log& setLine(const uint& line) {
		line_ = line;
		return *this;
	}

private:
	template <typename T>
	void _logText(const T& v, const true_type& _) {
		ss_ << v;
	}
	template <typename T>
	void _logText(const T& v, const false_type& _) {
		ss_ << typeid(v).name();
	}
	template <typename T>
	void _logText(const ObjectBase<T>& v, const false_type& _) {
		ss_ << typeid(v).name() << "[" << v.toString() << "]";
	}
	void _logText(const string& v, const false_type& _) {
		ss_ << v;
	}

private:
	std::stringstream			ss_;
	time_type					time_;
	LogLevel					level_;
	uuid_t						key_;

	string						filename_;
	uint						line_;

private:
	io_handler_type				io_handler_;
};

}}

/**
 * LOG
 * Sample:
 *   LOG(ERROR) << "Hello World" << End;
 */
#define ZCLOG(level)			::zertcore::log::Log::Instance().setFilename(__FILE__).setLine(__LINE__).setLevel(level)


#endif /* LOG_H_ */
