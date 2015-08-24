/*
 * EventBusImpl.hpp
 *
 *  Created on: 2015年8月18日
 *      Author: Administrator
 */

#ifndef ZERTCORE_SUIT_EVENT_DETAILS_EVENTBUSIMPL_HPP_
#define ZERTCORE_SUIT_EVENT_DETAILS_EVENTBUSIMPL_HPP_

#include "../EventBus.h"

namespace zertcore { namespace suit {

template <class E>
void EventBus<E>::
dispatch(event_type& e) {
	for (auto it = event_set_.begin(); it != event_set_.end(); ++it) {
		(*it)->onEvent(e);

		// check for the event's status
	}
}


template <class E>
void EventBus<E>::
addHandler(typename EventHandler<event_type>::ptr handler) {
	event_set_.insert(handler);
}


template <class E>
void EventBus<E>::
removeHandler(typename EventHandler<event_type>::ptr handler) {
	event_set_.erase(handler);
}

}}


#endif /* SUIT_EVENT_DETAILS_EVENTBUSIMPL_HPP_ */
