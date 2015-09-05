/*
 * client1.cpp
 *
 *  Created on: 2015年8月14日
 *      Author: Administrator
 */

#include <cstdio>
#include <string.h>
#include <enet/enet.h>

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
