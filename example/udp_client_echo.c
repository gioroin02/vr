#include <vr_system_memory.h>
#include <vr_system_socket.h>

#include <stdio.h>

#define INFO "[\x1b[34m  INFO \x1b[0m] "

int main(int args_count, char* args_array[])
{
    VR_Arena_Alloc arena = vr_memory_reserve(16, 1024);

    VR_Socket_Udp socket = vr_socket_udp_reserve((VR_Alloc*) &arena);

    VR_Network_Ip_Addr server_addr = vr_network_ip_addr_ver4(
        VR_NETWORK_IP_ADDR_VER4_LOCAL, 37134);

    vr_socket_udp_init(socket, server_addr.kind);

    char8  message[32] = {0};
    intptr count       = 0;

    count = snprintf(message, sizeof message, "%s", "Ciao, sono il client!");

    vr_socket_udp_write_all(socket, (uint8*) message, count, server_addr);

    printf(INFO "Nuovo messaggio:\n");
    printf("    Inviato '%.*s'\n", count, message);

    VR_Network_Ip_Addr addr    = {0};
    bool32             success = 0;

    while (success == 0) {
        count = vr_socket_udp_read(socket,
            (uint8*) message, sizeof message, &addr);

        if (vr_network_ip_addr_is_equal(server_addr, addr) == 0)
            continue;

        printf("    Ricevuto '%.*s'\n", count, message);

        success = 1;
    }

    vr_socket_udp_uninit(socket);

    return 0;
}
