/*
 * SingleInstance.h
 *
 *  Created on: 2014Äê10ÔÂ13ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_SINGLEINSTANCE_H_
#define ZERTCORE_SINGLEINSTANCE_H_

#include <pch.h>
#include <utils/types.h>

namespace zertcore { namespace utils {

template <class Final>
class Singleton :
		noncopyable
{
public:
	virtual ~Singleton() {}

public:
	static inline Final& Instance() {
		pthread_once(&ponce_, &Singleton::__initInstance);
		return *p_instance_;
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

private:
	static void __initInstance() {
		if (!p_instance_) {
			p_instance_ = new Final();
		}
	}

private:
	static pthread_once_t		ponce_;
	static Final*				p_instance_;
};

template<typename T>
pthread_once_t					Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;

template <class Final>
Final*							Singleton<Final>::p_instance_ = null;

}}


#endif /* SINGLEINSTANCE_H_ */
