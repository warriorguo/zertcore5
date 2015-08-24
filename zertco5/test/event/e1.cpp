/*
 * e1.cpp
 *
 *  Created on: 2015年8月24日
 *      Author: Administrator
 */

#include <zertcore.h>

namespace zertcore {
using namespace zertcore::suit;

struct MoveEvent
{
	u32							x,
								y;
};

class Player :
		public EventHandler<MoveEvent>
{
public:
	void onEvent(MoveEvent& event) {
		::printf("Player receive:(%u, %u)\n", event.x, event.y);
	}

public:
	void move(u32 x, u32 y) {
		MoveEvent evt;
		evt.x = x; evt.y = y;

		dispatchEvent(evt);
	}
};


class Map :
		public EventHandler<MoveEvent>
{
public:
	void onEvent(MoveEvent& event) {
		::printf("map receive:(%u, %u)\n", event.x, event.y);
	}
};


}


int main() {
	using namespace zertcore;
	config.concurrent.thread_nums = 1;

	Player player1, player2;
	Map map;

	player1.move(1, 2);
	player2.move(2, 3);

	return 0;
}
