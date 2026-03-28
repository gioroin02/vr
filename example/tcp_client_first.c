#include "../src/vr_sys_memory/export.h"
#include "../src/vr_sys_socket/export.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int params_count, char* params_array[])
{
    srand(time(NULL));

    VR_Arena_Alloc arena = vr_memory_reserve(16, VR_MEMORY_KIB);
    VR_Alloc       alloc = vr_alloc_arena(&arena);

    VR_Socket_TCP* socket = vr_socket_tcp_reserve(alloc);

    vr_socket_tcp_create(socket, vr_endpoint_ip4_empty());
    vr_socket_tcp_connect(socket, vr_endpoint_ip4_local(37134));

    uint8  message[32] = {0};
    intptr count       = 0;

    count = snprintf((char*) message, sizeof message,
        "Ciao, sono il client %u", rand() % 100000);

    vr_socket_tcp_write(socket, message, count);

    count = vr_socket_tcp_read(socket, message, sizeof message);

    printf("%.*s\n", (int) count, message);

    vr_socket_tcp_destroy(socket);

    return 0;
}
