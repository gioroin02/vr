#include <vr_system_memory.h>
#include <vr_system_socket.h>

#include <stdio.h>
#include <stdlib.h>

int main(int args_count, char* args_array[])
{
    VR_Arena_Alloc arena = vr_memory_reserve(16, VR_INTPTR_KIBI);
    VR_Alloc       alloc = vr_alloc_arena(&arena);

    VR_Socket_TCP* listener = vr_socket_tcp_reserve(alloc);
    VR_Socket_TCP* socket   = vr_socket_tcp_reserve(alloc);

    vr_socket_tcp_init_bound(listener, VR_Endpoint_IP_Kind_V4, 37134);
    vr_socket_tcp_listen(listener);

    vr_socket_tcp_accept(socket, listener);

    intptr number = 0;

    while (1) {
        uint8  message[32]      = {0};
        intptr count            = 0;
        uint8* message_end_pntr = NULL;
        intptr message_number   = 0;

        count = vr_socket_tcp_read(socket, message, sizeof message);

        printf("[INFO] Ricevuto '%.*s'\n", (int) count, message);

        message_number = strtoll((char*) message, (char**) &message_end_pntr, 10);

        if (message_end_pntr != message) number += message_number;

        count = snprintf((char*) message, sizeof message, "%lli", number);

        vr_socket_tcp_write(socket, message, count);

        printf("[INFO] Inviato '%.*s'\n", (int) count, message);

        if (message_number == 0) break;
    }

    vr_socket_tcp_deinit(socket);
    vr_socket_tcp_deinit(listener);

    return 0;
}
