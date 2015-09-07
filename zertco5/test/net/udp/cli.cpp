/*
 * cli.cpp
 *
 *  Created on: 2015年9月5日
 *      Author: Administrator
 */

#include <cstdio>
#include <string.h>
#include <enet/enet.h>
#include <pthread.h>

#include <boost/timer/timer.hpp>
#include <boost/function.hpp>
using boost::function;

/**
 * Client
 */
class Client
{
public:
	typedef function<void (Client&)>		connection_handler_type;
	typedef function<void (Client&, const char* data, int length)>
											receive_handler_type;

public:
	Client(int channel = 2, int downstream = 57600 / 8, int upstream = 14400 / 8) : peer_(NULL) {
		client_ = enet_host_create (NULL, 1, channel, downstream, upstream);
	}
	virtual ~Client() {
		enet_host_destroy(client_);
	}

public:
	bool tryConnect(const std::string& host, const int& port) {
		ENetAddress address;
		enet_address_set_host (& address, host.c_str());
		address.port = port;

		peer_ = enet_host_connect (client_, & address, 2, 0);
		return peer_? true: false;
	}
	bool send(const char* data, uint length) {
		if (!peer_) return false;

		uint buf_length = sizeof(short) + length;
		char* buf = new char[buf_length];

		*((unsigned short *)buf) = buf_length;
		memcpy(&buf[sizeof(short)], data, length);

    	ENetPacket * packet = enet_packet_create(buf, buf_length, ENET_PACKET_FLAG_RELIABLE);
    	enet_peer_send(peer_, 0, packet);
    	enet_host_flush(client_);

		delete[] buf;

		return true;
	}

public:
	void onConnected(const connection_handler_type& handler) {
		connected_handler_ = handler;
	}
	void onDisconnected(const connection_handler_type& handler) {
		disconnected_handler_ = handler;
	}
	void onReceived(const receive_handler_type& handler) {
		receive_handler_ = handler;
	}

public:
	void pollOnce(uint ms = 0) {
		ENetEvent event;
		if (enet_host_service (client_, & event, ms) > 0) {
			switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT:
				if (connected_handler_)
					connected_handler_(*this);
				break;

			case ENET_EVENT_TYPE_RECEIVE:
				if (receive_handler_) {
					receive_handler_(*this, (const char *)event.packet -> data, event.packet -> dataLength);
				}
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				if (disconnected_handler_)
					disconnected_handler_(*this);
				break;

			default:
				break;
			}

		}
	}

private:
	ENetHost*					client_;
	ENetPeer*					peer_;

private:
	connection_handler_type		connected_handler_,
								disconnected_handler_;
	receive_handler_type		receive_handler_;
};

void* work(void* _h) {
	using boost::timer::cpu_timer;
	using boost::timer::cpu_times;

	const char* host = (const char *)_h;
	uint times = 10000;
	cpu_timer timer;

	Client client;
	const char* str = "hello world";
	int length = strlen(str) + 1;
	ulong cost_time = 0;
	cpu_times last_times;

	client.onConnected([str, length, &last_times, &timer] (Client& c) {
		last_times = timer.elapsed();
		c.send(str, length);
	});

	client.onReceived([str, length, &cost_time, &last_times, &timer, &times] (Client& c, const char* data, int length) {
		if (times == 0) {
			return ;
		}

		--times;
		cpu_times now(timer.elapsed());
		cost_time += (now.system - last_times.system) + (now.user - last_times.user) + (now.wall - last_times.wall);

		usleep(1);
		last_times = timer.elapsed();
		c.send(str, length);
	});

	if (!client.tryConnect(host, 80)) {
		::printf("Connect failed\n");
		return NULL;
	}


	timer.start();
	uint last_v = 0;
	while (times > 0) {
		if (times % 10 == 0 && times != last_v) {
			last_v = times;
			::printf("Times:%u\n", times);
		}

		client.pollOnce(1);
	}

	::printf("Cost: %u\n", cost_time);
	return NULL;
}

#define MAX_TID								1

int main(int argc, char* argv[]) {
	if (argc != 2) {
		::printf("Usage:%s host\n", argv[0]);
		return 1;
	}

	pthread_t tids[MAX_TID];

	for (uint i = 0; i < MAX_TID; ++i) {
		pthread_create(&tids[i], NULL, work, argv[1]);
	}

	for (uint i = 0; i < MAX_TID; ++i)
		pthread_join(tids[i], NULL);

	return 0;
}


