#include "../src/vr_sys_memory/export.h"
#include "../src/vr_sys_socket/export.h"

#include <stdio.h>

int main(int params_count, char* params_array[])
{
    VR_Arena_Alloc arena = vr_memory_reserve(16, VR_MEMORY_KIB);
    VR_Alloc       alloc = vr_alloc_arena(&arena);

    VR_Socket_TCP* listener = vr_socket_tcp_reserve(alloc);
    VR_Socket_TCP* socket   = vr_socket_tcp_reserve(alloc);

    vr_socket_tcp_create(listener, vr_endpoint_ip4_local(37134));

    vr_socket_tcp_bind(listener);
    vr_socket_tcp_listen(listener);

    intptr sessions = 3;

    for (intptr i = 0; i < sessions; i += 1) {
        vr_socket_tcp_accept(socket, listener);

        uint8  message[32] = {0};
        intptr count       = 0;

        count = vr_socket_tcp_read(socket, message, sizeof message);

        printf("%.*s\n", (int) count, message);

        vr_socket_tcp_write(socket, message, count);
        vr_socket_tcp_destroy(socket);
    }

    vr_socket_tcp_destroy(listener);

    return 0;
}
