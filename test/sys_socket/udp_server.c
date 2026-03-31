#include "../../src/vr_sys_socket/export.h"

#include <stdio.h>

int main(int params_count, char* params_array[])
{
    uint8 memory[VR_INTPTR_KIBI] = {0};

    VR_Arena_Alloc arena = vr_arena_alloc_make(memory, sizeof memory);
    VR_Alloc       alloc = vr_alloc_arena(&arena);

    VR_Socket_UDP* socket   = vr_socket_udp_reserve(alloc);
    VR_Endpoint_IP endpoint = vr_endpoint_ip_none();

    vr_socket_udp_create(socket, vr_endpoint_ip4_local(5000));
    vr_socket_udp_bind(socket);

    char message[32] = {0};

    intptr count = vr_socket_udp_read(socket,
        (uint8*) message, sizeof message, &endpoint);

    printf("[INFO] Ricevuto '%.*s'\n", (int) count, message);

    vr_socket_udp_write(socket, (uint8*) message, count, endpoint);

    printf("[INFO] Inviato '%.*s'\n", (int) count, message);

    vr_socket_udp_destroy(socket);

    return 0;
}
