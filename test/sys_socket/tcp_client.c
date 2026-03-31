#include "../../src/vr_sys_socket/export.h"

#include <stdio.h>
#include <string.h>

int main(int params_count, char* params_array[])
{
    uint8 memory[VR_MEMORY_KIB] = {0};

    VR_Arena_Alloc arena = vr_arena_alloc_make(memory, sizeof memory);
    VR_Alloc       alloc = vr_alloc_arena(&arena);

    VR_Socket_TCP* socket = vr_socket_tcp_reserve(alloc);

    vr_socket_tcp_create(socket, vr_endpoint_ip4_empty());
    vr_socket_tcp_connect(socket, vr_endpoint_ip4_local(5000));

    char   message[32] = "Ciao";
    intptr count       = strlen(message);

    vr_socket_tcp_write(socket, (uint8*) message, count);

    printf("[INFO] Inviato '%.*s'\n", (int) count, message);

    count = vr_socket_tcp_read(socket, (uint8*) message, sizeof message);

    printf("[INFO] Ricevuto '%.*s'\n", (int) count, message);

    vr_socket_tcp_destroy(socket);

    return 0;
}
