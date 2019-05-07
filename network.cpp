#include "network.h"

#include <enet/enet.h>
#include <cstdlib>
#include <cstdio>
#include <map>
#include <deque>

static ENetHost *server, *client;
static ENetPeer *client_peer;
static bool is_connected;

static ENetPeer *peers[NETWORK_MAX_CLIENTS];
static std::map<ENetPeer *, int> clients;
static std::deque<int> client_ids;


static ENetPacket *encode_packet(uint8_t packet, void *data, size_t size) {
	size_t pksize = 1 + size;
	auto pk = new unsigned char[pksize];

	pk[0] = packet;
	memcpy(pk + 1, data, size);

	return enet_packet_create(pk, pksize, ENET_PACKET_FLAG_RELIABLE);
}

void network_init() {
	if (enet_initialize() != 0) {
		fprintf(stderr, "An error occurred while initializing ENet.\n");
		abort();
	}
	atexit(enet_deinitialize);

	server = client = nullptr;
	client_peer = nullptr;
	is_connected = false;

	for (int i = 0; i < NETWORK_MAX_CLIENTS; ++i) {
		client_ids.push_back(i);
		peers[i] = nullptr;
	}
}

void network_update() {
	ENetEvent event;

	if (client != nullptr) {
		while (enet_host_service(client, &event, 0) > 0) {
			switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT:
				network_on_client_connected();
				break;
			case ENET_EVENT_TYPE_RECEIVE: {
        auto buffer = reinterpret_cast<const unsigned char *>(event.packet->data);
        auto packet = static_cast<uint8_t>(buffer[0]);
        auto data = reinterpret_cast<const void *>(buffer + 1);
				network_on_client_message(packet, data);

				enet_packet_destroy(event.packet);
				break;
			}
			case ENET_EVENT_TYPE_DISCONNECT:
				network_on_client_disconnected();
			case ENET_EVENT_TYPE_NONE:
				break;
			}
		}

		enet_host_flush(client);
	}

	if (server != nullptr) {
		while (enet_host_service(server, &event, 0) > 0) {
			switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT: {
				int id = client_ids.front();
				client_ids.pop_front();

				peers[id] = event.peer;
				clients[event.peer] = id;

				network_on_server_added(id);
				break;
			}
			case ENET_EVENT_TYPE_RECEIVE: {
				int id = clients[event.peer];

        auto buffer = reinterpret_cast<const unsigned char *>(event.packet->data);
        auto packet = static_cast<uint8_t>(buffer[0]);
        auto data = reinterpret_cast<const void *>(buffer + 1);
				network_on_server_message(id, packet, data);

				enet_packet_destroy(event.packet);
				break;
			}
			case ENET_EVENT_TYPE_DISCONNECT: {
				int id = clients[event.peer];

				clients.erase(event.peer);
				peers[id] = nullptr;

				network_on_server_removed(id);
				break;
			}
			case ENET_EVENT_TYPE_NONE:
				break;
			}
		}

		enet_host_flush(server);
	}
}

void network_connect(const char *host, int port) {
	network_disconnect();

	client = enet_host_create(nullptr, 1, 1, 0, 0);
	if (client == nullptr) {
		fprintf(stderr, "An error occurred while trying to create an ENet client host.\n");
		abort();
	}

	ENetAddress address;
	enet_address_set_host(&address, host);
	address.port = static_cast<enet_uint16>(port);

	client_peer = enet_host_connect(client, &address, 1, 0);
}

void network_disconnect() {
	if (client_peer != nullptr) {
		// TODO: Call disconnected

		enet_peer_disconnect(client_peer, 0);
		client_peer = nullptr;
	}

	if (client != nullptr) {
		enet_host_destroy(client);
		client = nullptr;
	}
}

bool network_is_connected() {
	return is_connected;
}

void network_client_send(uint8_t packet, void *data, size_t size) {
	enet_peer_send(client_peer, 0, encode_packet(packet, data, size));
}


void network_start_server(int port) {
	network_stop_server();

	ENetAddress address;
	address.host = ENET_HOST_ANY;
	address.port = static_cast<enet_uint16>(port);

	server = enet_host_create(&address, NETWORK_MAX_CLIENTS, 1, 0, 0);
	if (server == nullptr) {
		fprintf(stderr, "An error occurred while trying to create an ENet server host.\n");
		abort();
	}
}

void network_stop_server() {
	if (server == nullptr) {
		return;
	}

	enet_host_destroy(server);
	server = nullptr;
}

bool network_has_server() {
	return server != nullptr;
}

void network_server_broadcast(uint8_t packet, void *data, size_t size) {
	enet_host_broadcast(server, 0, encode_packet(packet, data, size));
}

void network_server_send(int client, uint8_t packet, void *data, size_t size) {
	auto peer = peers[client];
	if (peer != nullptr) {
		enet_peer_send(peer, 0, encode_packet(packet, data, size));
	}
}
