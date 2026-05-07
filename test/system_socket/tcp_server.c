#include <vr_system_socket.h>
#include <stdio.h>

int main(int args_count, char* args_array[])
{
    uint8 memory[VR_INTPTR_KIBI] = {0};

    VR_Arena_Alloc arena = vr_arena_alloc_make(memory, sizeof memory);
    VR_Alloc       alloc = vr_alloc_arena(&arena);

    VR_Socket_TCP listener = vr_socket_tcp_reserve(alloc);
    VR_Socket_TCP socket   = vr_socket_tcp_reserve(alloc);

    vr_socket_tcp_init_bound(listener, VR_Endpoint_IP_Kind_V4, 5000);
    vr_socket_tcp_listen(listener);

    vr_socket_tcp_accept(socket, listener);

    char message[32] = {0};

    intptr count = vr_socket_tcp_read(socket, (uint8*) message, sizeof message);

    printf("[INFO] Ricevuto '%.*s'\n", (int) count, message);

    vr_socket_tcp_write(socket, (uint8*) message, count);

    printf("[INFO] Inviato '%.*s'\n", (int) count, message);

    vr_socket_tcp_deinit(socket);
    vr_socket_tcp_deinit(listener);

    return 0;
}
