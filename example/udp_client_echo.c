#include <vr_system_memory.h>
#include <vr_system_socket.h>

#include <stdio.h>

#define INFO "[\x1b[34m  INFO \x1b[0m]"

int main(int args_count, char* args_array[])
{
    VR_Arena_Alloc arena = vr_memory_reserve(16, 1024);

    VR_Socket_UDP  socket = vr_socket_udp_reserve((VR_Alloc*) &arena);
    VR_Endpoint_IP server = vr_endpoint_ip_ver4(VR_ENDPOINT_IPV4_LOCAL, 37134);

    vr_socket_udp_init(socket, server.kind);

    char8  message[32] = {0};
    intptr count       = 0;

    count = snprintf(message, sizeof message, "%s", "Ciao, sono il client!");

    vr_socket_udp_write_all(socket, (uint8*) message, count, server);

    printf(INFO " Inviato '%.*s'\n", (int) count, message);

    VR_Endpoint_IP endpoint = {0};
    bool32         success  = 0;

    while (success == 0) {
        count = vr_socket_udp_read(socket,
            (uint8*) message, sizeof message, &endpoint);

        if (vr_endpoint_ip_is_equal(server, endpoint) == 0)
            continue;

        printf(INFO " Ricevuto '%.*s'\n", (int) count, message);

        success = 1;
    }

    vr_socket_udp_uninit(socket);

    return 0;
}
