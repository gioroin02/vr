#include <vr_system_socket.h>

#include <stdio.h>
#include <string.h>

int main(int args_count, const char* args_array[])
{
    uint8 memory[VR_INTPTR_KILO_2] = {0};

    VR_ArenaAlloc arena = vr_arena_alloc_make(memory, sizeof memory);

    VR_NetworkIpAddr addr = vr_network_ip_addr_ver4(
        VR_NETWORK_IP_ADDR_VER4_LOCAL, 5000);

    VR_SocketTcp socket = vr_socket_tcp_reserve((VR_Alloc*) &arena);

    vr_socket_tcp_init(socket, addr.kind);
    vr_socket_tcp_connect(socket, addr);

    char   message[32] = "Ciao!";
    intptr count       = strlen(message);

    vr_socket_tcp_write(socket, (uint8*) message, count);

    printf("[INFO] Inviato '%.*s'\n", count, message);

    count = vr_socket_tcp_read(socket, (uint8*) message, sizeof message);

    printf("[INFO] Ricevuto '%.*s'\n", count, message);

    vr_socket_tcp_uninit(socket);

    return 0;
}
