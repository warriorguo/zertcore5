/*
 * Log.h
 *
 *  Created on: 2014��11��19��
 *      Author: Administrator
 */

#ifndef ZERTCORE_LOG_H_
#define ZERTCORE_LOG_H_

#include <pch.h>

#include <utils/types.h>
#include <utils/time/TimeType.h>
#include <thread/ThreadSingleton.h>

#include <object/ObjectBase.h>

namespace zertcore { namespace log {
using namespace zertcore::utils;
using namespace zertcore::object;
using namespace zertcore::concurrent;
}}

namespace zertcore {

/**
 * Log level
 */
enum LogLevel
{
	NONE									= 0,
	NOTE									= 1,
	DEBUG									= NOTE + 1,
	NOTICE									= DEBUG + 1,
	WARNING									= NOTICE + 1,
	ERROR									= WARNING + 1,
	FATAL									= ERROR + 1,

	LEVEL_MAX								= FATAL,
};

extern const char* LogLevelNameMap[];

}

namespace zertcore { namespace log { namespace details {

/**
 * LogData
 */
struct LogData
{
	LogData() : level(NONE), key(0), line(0) {}
	void clear() {level = NONE; key = 0; line = 0;filename.clear();func.clear();}

	LogLevel					level;
	uuid_t						key;
	uint						line;
	time_type					time;
	string						filename;
	string						func;
};

}}}

namespace zertcore { namespace log {

typedef details::LogData					LogDetails;

void STDIN_W(const LogDetails& data, const string& log);

/**
 * Log,
 * FIXME: support for multi-thread
 */
class Log :
		public ThreadSingleton<Log>
{
public:
	typedef signals2::signal<void (const LogDetails&,
			const string& log)>				io_handler_type;
	typedef signals2::connection			io_connection_type;

public:
	Log() : set_error_ptr_(NULL) {}

public:
	static bool setup();

public:
	/**
	 * IMPORTANT: handler itself must be thread safe!
	 */
	template <typename HANDLE>
	static io_connection_type setIO(HANDLE handler) {
		return io_handlers_.connect(handler);
	}

public:
	template <typename T>
	Log& operator << (const T& v) {
		_logText(v, is_arithmetic<T>());
		return *this;
	}

	template <typename T>
	Log& operator << (const SMART_PTR(T)& v) {
		return operator << (*v);
	}

	Log& operator << (const char* v) {
		ss_ << v;
		return *this;
	}

	Log& operator << (const string& v) {
		ss_ << v;
		return *this;
	}

public:
	/**
	 * the end to this record of log
	 */
	void operator << (const __END& v);

public:
	/**
	 * Just working for the next Error Log
	 */
	Log& operator >> (Error& error) {
		set_error_ptr_ = &error;
		return *this;
	}

public:
	Log& setLevel(const LogLevel& level) {
		data_.level = level;
		return *this;
	}
	Log& setKey(const uuid_t& key) {
		data_.key = key;
		return *this;
	}
	Log& setFilename(const string& filename) {
		data_.filename = filename;
		return *this;
	}
	Log& setLine(const uint& line) {
		data_.line = line;
		return *this;
	}
	Log& setFunction(const string& func) {
		data_.func = func;
		return *this;
	}

private:
	template <typename T>
	void _logText(const T& v, const true_type& _) {
		ss_ << v;
	}
	template <typename T>
	void _logText(const T& v, const false_type& _) {
		_logObject(v, is_base_of<ObjectBase<T>, T>());
	}
	template <typename T>
	void _logObject(const T& v, const true_type& _) {
		ss_ << typeid(v).name() << "[" << v.toString() << "]";
	}
	template <typename T>
	void _logObject(const T& v, const false_type& _) {
		ss_ << typeid(v).name();
	}

private:
	std::stringstream			ss_;
	LogDetails					data_;

private:
	Error*						set_error_ptr_;

private:
	static io_handler_type		io_handlers_;
};

}}

/**
 * LOG
 * Sample:
 *   ZCLOG(ERROR) << "Hello World" << End;
 */
#define ZCLOG(level)			::zertcore::log::Log::Instance().\
			setFilename(__FILE__).setLine(__LINE__).setFunction(__PRETTY_FUNCTION__).setLevel(::zertcore::level)
#define ZCLOG_SETUP()			::zertcore::log::Log::setup()


#endif /* LOG_H_ */
