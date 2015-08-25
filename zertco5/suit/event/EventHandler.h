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
 * EventHandler<EventDerived>
 */
template <class EventDerived>
class EventHandler
{
public:
	typedef EventHandler<EventDerived>*		ptr;

public:
	EventHandler();
	virtual ~EventHandler();

public:
	void addListener();
	void removeListener();

public:
	virtual void onEvent(EventDerived& event)
											= 0;
};

}}



#endif /* SUIT_EVENT_EVENTHANDLER_H_ */
