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
	ERROR									= NOTICE + 1,
	FINAL									= ERROR + 1,

	LEVEL_MAX								= FINAL,
};

extern const char* LogLevelNameMap[];

}

namespace zertcore { namespace log {

void STDIN_W(const LogLevel& level, const time_type& time,
			const string& filename, const uint& line, const uuid_t& key, const string& log);

/**
 * Log,
 * FIXME: support for multi-thread
 */
class Log :
		public ThreadSingleton<Log>
{
public:
	typedef signals2::signal<void (const LogLevel& level, const time_type& time,
			const string& filename, const uint& line, const uuid_t& key, const string& log)>
											io_handler_type;
	typedef signals2::connection			io_connection_type;

public:
	Log() : level_(NONE), key_(0), line_(0), set_error_ptr_(NULL) {}

public:
	static bool init();

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

public:
	/**
	 * the end to this record of log
	 */
	void operator << (const __END& v) {
		string errmsg = move(ss_.str());

		if (io_handlers_) {
			time_.getTime();
			io_handlers_(level_, time_, filename_, line_, key_, errmsg);
		}

		if (level_ >= ERROR && set_error_ptr_) {
			set_error_ptr_->setError(errmsg);
		}

		ss_.str(string());
		ss_.clear();

		set_error_ptr_ = NULL;

		/**
		 * if the log level was final error, throw the error
		 */
		if (level_ == FINAL) {
			throw Error(ss_.str());
		}
	}

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
	time_type					time_;
	LogLevel					level_;
	uuid_t						key_;

	string						filename_;
	uint						line_;

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
			setFilename(__FILE__).setLine(__LINE__).setLevel(::zertcore::level)
#define ZCLOG_SETUP()			::zertcore::log::Log::init()


#endif /* LOG_H_ */
