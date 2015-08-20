/*
 * EventHandlerImpl.hpp
 *
 *  Created on: 2015年8月18日
 *      Author: Administrator
 */

#ifndef ZERTCORE_SUIT_EVENT_DETAILS_EVENTHANDLERIMPL_HPP_
#define ZERTCORE_SUIT_EVENT_DETAILS_EVENTHANDLERIMPL_HPP_

#include "../EventHandler.h"
#include "../EventBus.h"

namespace zertcore { namespace suit {

template <class Event>
EventHandler<Event>::EventHandler() {
	EventBus<Event>::Instance().addHandler(this);
}

template <class Event>
EventHandler<Event>::~EventHandler() {
	EventBus<Event>::Instance().removeHandler(this);
}

}}


#endif /* SUIT_EVENT_DETAILS_EVENTHANDLERIMPL_HPP_ */
