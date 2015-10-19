/*
 * test1.cpp
 *
 *  Created on: 2015年4月16日
 *      Author: Administrator
 */
#include <stdio.h>
#include <enet/enet.h>
#include <stdlib.h>
#include <string.h>

#include <map>

int main() {
	if (enet_initialize () != 0) {
		fprintf (stderr, "An error occurred while initializing ENet.\n");
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);

	ENetAddress address;
	ENetHost* server;

	address.host = ENET_HOST_ANY;
	address.port = 1234;

	server = enet_host_create(&address, 1024, 2, 0, 0);
	if (server == NULL) {
		fprintf (stderr, "An error occurred while trying to create an ENet server host.\n");
		return EXIT_FAILURE;
	}

	std::map<long, ENetPeer*> all_clients;
	long index = 1;

	do {
		ENetEvent event;
		while (enet_host_service (server, & event, 1000) > 0) {
			switch (event.type)  {
			case ENET_EVENT_TYPE_CONNECT:
				printf ("A new client connected from %x:%u.\n",
						event.peer -> address.host,
						event.peer -> address.port);
				/* Store any relevant client information here. */
				event.peer->data = (void *)index;
				all_clients[index] = event.peer;
				index++;
				break;

			case ENET_EVENT_TYPE_RECEIVE: {
				long index = (long)event.peer -> data;
				printf ("A packet of length %u containing %s was received from %ld on channel %u.\n",
						event.packet -> dataLength,
						event.packet -> data,
						index,
						event.channelID);
				/* Clean up the packet now that we're done using it. */
				for(auto it = all_clients.begin(); it != all_clients.end(); ++it) {
					enet_peer_send (it->second, 0, event.packet);
				}

				enet_host_flush(server);
				break;
			}

			case ENET_EVENT_TYPE_DISCONNECT: {
				/* Reset the peer's client information. */
				long index = (long)event.peer -> data;
				printf ("%ld disconnected.\n", index);
				event.peer -> data = NULL;

				all_clients.erase(index);
				break;
			}

			}
		}
	}
	while(true);

	enet_host_destroy(server);
	printf("Exit\n");
	return 0;
}
