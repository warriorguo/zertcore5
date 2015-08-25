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

template <class E>
EventHandler<E>::EventHandler() {
	addListener();
}

template <class E>
EventHandler<E>::~EventHandler() {
	removeListener();
}

template <class E>
void EventHandler<E>::
addListener() {
	EventBus<E>::Instance().addHandler(this);
}

template <class E>
void EventHandler<E>::
removeListener() {
	EventBus<E>::Instance().removeHandler(this);
}

}}


#endif /* SUIT_EVENT_DETAILS_EVENTHANDLERIMPL_HPP_ */
