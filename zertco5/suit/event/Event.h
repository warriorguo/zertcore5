/*
 * Event.h
 *
 *  Created on: 2015年7月3日
 *      Author: Administrator
 */

#ifndef ZERTCORE_SUIT_EVENT_EVENT_H_
#define ZERTCORE_SUIT_EVENT_EVENT_H_

#include <pch.h>
#include <utils/types.h>

namespace zertcore { namespace suit {

/**
 * inspired by danquist's EventBus.
 * 	@https://github.com/danquist/EventBus
 */

/**
 * Event
 */
class Event
{
public:
	enum {
		NONE								= 0,
		SENDING								= 1,
		STOPPED								= 2,
		FINISH								= 3,
		ERROR								= 10,
	};
	typedef u32								status_type;

private:
	status_type					status_{NONE};
};

}}


#endif /* SUIT_EVENT_EVENT_H_ */
