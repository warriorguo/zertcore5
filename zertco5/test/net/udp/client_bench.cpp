/*
 * client_bench.cpp
 *
 *  Created on: 2015年9月4日
 *      Author: Administrator
 */

#include <cstdio>
#include <string.h>
#include <enet/enet.h>

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

		peer_ = enet_host_connect (client, & address, 2, 0);
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
		if (enet_host_service (client_, & event, 1000) > 0) {
			switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT:
				if (connected_handler_)
					connected_handler_(*this);
				break;

			case ENET_EVENT_TYPE_RECEIVE:
				if (receive_handler_) {
					receive_handler_(*this, event.packet -> data, event.packet -> dataLength);
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


