#include <vr_system_socket.h>

#include <stdio.h>

int main(int args_count, const char* args_array[])
{
    uint8 memory[VR_INTPTR_KILO_2] = {0};

    VR_ArenaAlloc arena = vr_arena_alloc_make(memory, sizeof memory);

    VR_SocketTcp listener = vr_socket_tcp_reserve((VR_Alloc*) &arena);
    VR_SocketTcp socket   = vr_socket_tcp_reserve((VR_Alloc*) &arena);

    vr_socket_tcp_init_bound(listener, VR_NetworkIpAddr_Kind_Ver4, 5000);

    vr_socket_tcp_listen(listener);

    vr_socket_tcp_accept(socket, listener);

    char message[32] = {0};

    intptr count = vr_socket_tcp_read(socket, (uint8*) message, sizeof message);

    printf("[INFO] Ricevuto '%.*s'\n", count, message);

    vr_socket_tcp_write(socket, (uint8*) message, count);

    printf("[INFO] Inviato '%.*s'\n", count, message);

    vr_socket_tcp_uninit(socket);
    vr_socket_tcp_uninit(listener);

    return 0;
}
