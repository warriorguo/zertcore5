/*
 * Config.h
 *
 *  Created on: 2014Äê11ÔÂ12ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_CONFIG_H_
#define ZERTCORE_CONFIG_H_

#include <pch.h>
#include <utils/types.h>

#include <concurrent/config.h>
#include <serialize/config.h>

namespace zertcore{

/**
 * Config
 */
struct Config
{
	concurrent::ConcurrentConfig
								concurrent;
};

extern Config					config;

}



#endif /* CONFIG_H_ */
