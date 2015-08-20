/*
 * EventHandler.h
 *
 *  Created on: 2015年8月18日
 *      Author: Administrator
 */

#ifndef ZERTCORE_SUIT_EVENT_EVENTHANDLER_H_
#define ZERTCORE_SUIT_EVENT_EVENTHANDLER_H_

#include <pch.h>
#include <utils/types.h>

namespace zertcore { namespace suit {

/**
 * EventHandler<F>
 */
template <class Event>
class EventHandler
{
public:
	typedef EventHandler*					ptr;

public:
	EventHandler();
	virtual ~EventHandler();

public:
	virtual void onEvent(Event& event)		= 0;
};

}}



#endif /* SUIT_EVENT_EVENTHANDLER_H_ */
