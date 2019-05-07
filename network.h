#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

#define NETWORK_MAX_CLIENTS 32

/////////////////////////////
// Client
void network_on_client_connected();
void network_on_client_disconnected();
void network_on_client_message(uint8_t packet, const void *data);

// Server
void network_on_server_added(int i);
void network_on_server_removed(int i);
void network_on_server_message(int i, uint8_t packet, const void *data);
/////////////////////////////

void network_init();
void network_update();

// Client
void network_connect(const char *host, int port);
void network_disconnect();
bool network_is_connected();
void network_client_send(uint8_t packet, void *data, size_t size);
template<class T> void network_client_send(uint8_t packet, T *data) {
	network_client_send(packet, data, sizeof(T));
}
template<class T> void network_client_send(T *data) {
  network_client_send(T::ID, data, sizeof(T));
}

// Server
void network_start_server(int port);
void network_stop_server();
bool network_has_server();

void network_server_broadcast(uint8_t packet, void *data, size_t size);
template<class T> void network_server_broadcast(uint8_t packet, T *data) {
	network_server_broadcast(packet, data, sizeof(T));
}
template<class T> void network_server_broadcast(T *data) {
  network_server_broadcast(T::ID, data, sizeof(T));
}

void network_server_send(int client, uint8_t packet, void *data, size_t size);
template<class T> void network_server_send(int client, uint8_t packet, T *data) {
	network_server_send(client, packet, data, sizeof(T));
}
template<class T> void network_server_send(int client, T *data) {
  network_server_send(client, T::ID, data, sizeof(T));
}
