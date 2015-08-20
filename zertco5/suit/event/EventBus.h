/*
 * EventBus.h
 *
 *  Created on: 2015年8月18日
 *      Author: Administrator
 */

#ifndef ZERTCORE_SUIT_EVENT_EVENTBUS_H_
#define ZERTCORE_SUIT_EVENT_EVENTBUS_H_

#include <pch.h>
#include <utils/types.h>

#include <utils/Singleton.h>

#include "EventHandler.h"

namespace zertcore { namespace suit {
using namespace zertcore::utils;
}}

namespace zertcore { namespace suit {

/**
 * EventBus
 */
template <class Event>
class EventBus :
		public Singleton<EventBus<Event>, NoneChecker>
{
public:
	typedef Event							event_type;

public:
	typedef unordered_set<typename EventHandler<event_type>::ptr>
											handler_set_type;

public:
	/**
	 * dispatch actions
	 */
	void dispatch(event_type& e);

	/**
	 * add & remove Handler
	 */
	void addHandler(typename EventHandler<event_type>::ptr handler);
	void removeHandler(typename EventHandler<event_type>::ptr handler);

private:
	handler_set_type			event_set_;
};

}}


namespace zertcore {

/**
 * Helper functions
 */
template <class Event>
inline static void dispatchEvent(Event& e) {
	suit::EventBus<Event>::Instance().dispatch(e);
}

}

#endif /* SUIT_EVENT_EVENTBUS_H_ */
