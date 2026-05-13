#include <vr_system_socket.h>

#include <stdio.h>

int main(int args_count, const char* args_array[])
{
    uint8 memory[VR_INTPTR_KILO_2] = {0};

    VR_ArenaAlloc arena = vr_arena_alloc_make(memory, sizeof memory);

    VR_SocketUdp     socket = vr_socket_udp_reserve((VR_Alloc*) &arena);
    VR_NetworkIpAddr addr   = {0};

    vr_socket_udp_init_bound(socket, VR_NetworkIpAddr_Kind_Ver4, 5000);

    char message[32] = {0};

    intptr count = vr_socket_udp_read(socket,
        (uint8*) message, sizeof message, &addr);

    printf("[INFO] Ricevuto '%.*s'\n", count, message);

    vr_socket_udp_write(socket, (uint8*) message, count, addr);

    printf("[INFO] Inviato '%.*s'\n", count, message);

    vr_socket_udp_uninit(socket);

    return 0;
}
