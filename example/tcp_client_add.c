#include "../src/vr_sys_memory/export.h"
#include "../src/vr_sys_socket/export.h"

#include <stdio.h>
#include <stdlib.h>

int main(int params_count, char* params_array[])
{
    VR_Arena_Alloc arena = vr_memory_reserve(16, VR_MEMORY_KIB);
    VR_Alloc       alloc = vr_alloc_arena(&arena);

    VR_Socket_TCP* socket = vr_socket_tcp_reserve(alloc);

    vr_socket_tcp_create(socket, vr_endpoint_ip4_empty());
    vr_socket_tcp_connect(socket, vr_endpoint_ip4_local(37134));

    uint8  message[32]      = {0};
    intptr count            = 0;
    uint8* message_end_pntr = NULL;
    intptr message_number   = 0;

    while (1) {
        message_end_pntr = message;

        while (message == message_end_pntr) {
            printf("$ ");

            fgets((char*) message, sizeof message - 1, stdin);

            while (message[count] != 0) count += 1;

            message_number = strtoll((char*) message,
                (char**) &message_end_pntr, 10);
        }

        count = snprintf((char*) message, sizeof message,
            "%lli", message_number);

        vr_socket_tcp_write(socket, message, count);

        printf("[INFO] Inviato '%.*s'\n", (int) count, message);

        count = vr_socket_tcp_read(socket, message, sizeof message);

        printf("[INFO] Ricevuto '%.*s'\n", (int) count, message);

        if (message_number == 0) break;
    }

    vr_socket_tcp_destroy(socket);

    return 0;
}
