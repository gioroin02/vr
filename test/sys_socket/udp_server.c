#include "../../src/vr_sys_socket/export.h"

#include <stdio.h>

int main(int params_count, char* params_array[])
{
    uint8 buffer[VR_MEMORY_KIB] = {0};

    VR_Arena_Alloc arena = vr_arena_alloc_make(buffer, sizeof buffer);
    VR_Alloc       alloc = vr_alloc_arena(&arena);

    VR_Socket_UDP* socket   = vr_socket_udp_reserve(alloc);
    VR_Endpoint_IP endpoint = vr_endpoint_ip_none();

    vr_socket_udp_create(socket, vr_endpoint_ip4_local(5000));
    vr_socket_udp_bind(socket);

    char message[8] = {0};

    intptr count = vr_socket_udp_read(socket, (uint8*) message, sizeof message, &endpoint);

    printf("%.*s\n", (int) count, message);

    vr_socket_udp_write(socket, (uint8*) message, sizeof message, endpoint);

    vr_socket_udp_destroy(socket);

    return 0;
}
