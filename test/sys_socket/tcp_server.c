#include "../../src/vr_sys_socket/export.h"

#include <stdio.h>

int main(int params_count, char* params_array[])
{
    uint8 buffer[VR_MEMORY_KIB] = {0};

    VR_Arena_Alloc arena = vr_arena_alloc_make(buffer, sizeof buffer);
    VR_Alloc       alloc = vr_alloc_arena(&arena);

    VR_Socket_TCP* listener = vr_socket_tcp_reserve(alloc);
    VR_Socket_TCP* socket   = vr_socket_tcp_reserve(alloc);

    vr_socket_tcp_create(listener, vr_endpoint_ip4_local(5000));

    vr_socket_tcp_bind(listener);
    vr_socket_tcp_listen(listener);

    vr_socket_tcp_accept(socket, listener);

    char message[8] = {0};

    intptr count = vr_socket_tcp_read(socket, (uint8*) message, sizeof message);

    printf("[INFO] Ricevuto '%.*s'\n", (int) count, message);

    vr_socket_tcp_write(socket, (uint8*) message, count);

    printf("[INFO] Inviato '%.*s'\n", (int) count, message);

    vr_socket_tcp_destroy(socket);
    vr_socket_tcp_destroy(listener);

    return 0;
}
