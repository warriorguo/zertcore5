/*
 * client1.cpp
 *
 *  Created on: 2015年8月14日
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
	void send(const char* data, const int& length);

public:
	void onConnected(const connection_handler_type& handler);
	void onDisconnected(const connection_handler_type& handler);
	void onReceived(const receive_handler_type& handler);

public:
	void pollOnce(uint ms = 0) {
		ENetEvent event;
		if (enet_host_service (client_, & event, 1000) > 0) {
			switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT:
				if (connected_handler_) connected_handler_(*this);
				break;

			case ENET_EVENT_TYPE_RECEIVE:
				if (receive_handler_) {
					receive_handler_(*this, event.packet -> data, event.packet -> dataLength);
				}
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				if (disconnected_handler_) disconnected_handler_(*this);
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

ENetHost * client;

int main() {
	client = enet_host_create (NULL /* create a client host */,
	            1 /* only allow 1 outgoing connection */,
	            2 /* allow up 2 channels to be used, 0 and 1 */,
	            57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
	            14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);
	if (client == NULL) {
	    fprintf (stderr,
	             "An error occurred while trying to create an ENet client host.\n");
	    exit (EXIT_FAILURE);
	}

	ENetAddress address;
	ENetEvent event;
	ENetPeer *peer;

	enet_address_set_host (& address, "192.168.1.52");
	address.port = 80;

	peer = enet_host_connect (client, & address, 2, 0);
	if (peer == NULL)
	{
	   fprintf (stderr,
	            "No available peers for initiating an ENet connection.\n");
	   exit (EXIT_FAILURE);
	}

	/* Wait up to 1000 milliseconds for an event. */
	while (enet_host_service (client, & event, 1000) > 0)
	{
	    switch (event.type)
	    {
	    case ENET_EVENT_TYPE_CONNECT: {
	    	const char* msg = "{\"k\":\"key\",\"p\":0}";
	    	char cm[100];
	    	std::size_t length = strlen(msg) + sizeof(short);
	    	*((short *)cm) = length;

	    	strcpy(&cm[2], msg);

	    	ENetPacket * packet = enet_packet_create(cm, length, ENET_PACKET_FLAG_RELIABLE);
	    	enet_peer_send (peer, 0, packet);
	    	enet_host_flush (client);
	        break;
	    }
	    case ENET_EVENT_TYPE_RECEIVE:
	        printf ("A packet of length %u containing %s was received from %s on channel %u.\n",
	                event.packet -> dataLength,
	                event.packet -> data,
	                event.peer -> data,
	                event.channelID);
	        /* Clean up the packet now that we're done using it. */
	        enet_packet_destroy (event.packet);

	        break;

	    case ENET_EVENT_TYPE_DISCONNECT:
	        printf ("%s disconnected.\n", event.peer -> data);
	        /* Reset the peer's client information. */
	        event.peer -> data = NULL;
	    }
	}

	enet_host_destroy(client);
	return 0;
}
