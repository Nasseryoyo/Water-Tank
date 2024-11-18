#include <Drivers/Wifi/WifiServer.h>


int wifi_server_init(const char *ssid, int port) {
    if (cyw43_wifi_ap_set_up(ssid, NULL, CYW43_AUTH_OPEN, 1) != 0) {
        printf("Failed to set up access point\n");
        return -1;
    }
    printf("AP mode enabled: SSID = %s\n", ssid);

    if (cyw43_tcp_listen(port, &server_socket) != 0) {
        printf("Failed to start server on port %d\n", port);
        return -1;
    }
    printf("Server running on port %d\n", port);
    return 0;
}

struct cyw43_socket wifi_server_accept() {
    struct cyw43_socket client_socket = {0};
    if (cyw43_tcp_accept(&server_socket, &client_socket) == PICO_OK) {
        return client_socket;
    }
    return client_socket; // Will have `sock == 0` if invalid
}

int wifi_server_send(struct cyw43_socket *client_socket, const char *data) {
    return cyw43_tcp_send(client_socket, data, strlen(data));
}

void wifi_server_close(struct cyw43_socket *client_socket) {
    cyw43_tcp_close(client_socket);
}
