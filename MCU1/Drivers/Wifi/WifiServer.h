#pragma once

#include <pico/cyw43_arch.h>
#include <string.h>
#include <stdio.h>

static struct cyw43_socket server_socket;

int wifi_server_init(const char* ssid, int port);
struct cyw43_socket wifi_server_accept();
int wifi_server_send(struct cyw43_socket *client_socket, const char *data);
void wifi_server_close(struct cyw43_socket *client_socket);


