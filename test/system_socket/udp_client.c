#include <vr_system_socket.h>

#include <stdio.h>
#include <string.h>

int main(int args_count, const char* args_array[])
{
    uint8 memory[VR_INTPTR_KILO_2] = {0};

    VR_Arena_Alloc arena = vr_arena_alloc_make(memory, sizeof memory);

    VR_Socket_Udp socket = vr_socket_udp_reserve((VR_Alloc*) &arena);

    VR_Network_Ip_Addr addr = vr_network_ip_addr_ver4(
        VR_NETWORK_IP_ADDR_VER4_LOCAL, 5000);

    vr_socket_udp_init(socket, addr.kind);

    char   message[32] = "Ciao";
    intptr count       = strlen(message);

    vr_socket_udp_write(socket, (uint8*) message, count, addr);

    printf("[INFO] Inviato '%.*s'\n", count, message);

    count = vr_socket_udp_read(socket,
        (uint8*) message, sizeof message, &addr);

    printf("[INFO] Ricevuto '%.*s'\n", count, message);

    vr_socket_udp_uninit(socket);

    return 0;
}
