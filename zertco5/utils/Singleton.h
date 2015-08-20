/*
 * SingleInstance.h
 *
 *  Created on: 2014��10��13��
 *      Author: Administrator
 */

#ifndef ZERTCORE_SINGLEINSTANCE_H_
#define ZERTCORE_SINGLEINSTANCE_H_

#include <pch.h>
#include <utils/types.h>
#include <utils/tools/SameThreadChecker.h>
#include <thread/ThreadHandlerSet.h>

namespace zertcore { namespace utils {

/**
 * NoneChecker
 */
struct NoneChecker
{
	void init() {}
	void check() {}
};

/**
 * Singleton
 */
template <class Final, class Checker = SameThreadChecker>
class Singleton :
		public concurrent::ThreadHandlerSet<Final>,
		noncopyable
{
public:
	virtual ~Singleton() {}

public:
	virtual void init() {}

public:
	static inline Final& Instance() {
		pthread_once(&ponce_, &Singleton::__initInstance);
		return *p_instance_;
	}
	/**
	 * trigger the init();
	 */
	static void setup() {
		Instance();
	}

public:
	virtual bool deinit() {
		if (p_instance_) {
			delete p_instance_;
			p_instance_ = null;

			return true;
		}

		return false;
	}

protected:
	void threadCheck() {
		checker_.check();
	}

private:
	static void __initInstance() {
		if (!p_instance_) {
			p_instance_ = new Final();
			p_instance_->template initHandler(bind(&Final::__start, p_instance_));
		}
	}

private:
	void __start() {
		checker_.init();
		init();
	}

private:
	Checker						checker_;

private:
	static pthread_once_t		ponce_;
	static Final*				p_instance_;
};

template <class Final, class Checker>
pthread_once_t					Singleton<Final, Checker>::ponce_ = PTHREAD_ONCE_INIT;

template <class Final, class Checker>
Final*							Singleton<Final, Checker>::p_instance_ = null;

}}


#endif /* SINGLEINSTANCE_H_ */
