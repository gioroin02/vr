#include <vr_system_memory.h>
#include <vr_system_socket.h>

#include <stdio.h>
#include <stdlib.h>

intptr intptr_add_safe(intptr a, intptr b)
{
    if (b <= 0 || a > VR_INTPTR_MAX - b)
        return a;

    return a + b;
}

#define INFO "[\x1b[34m  INFO \x1b[0m]"

int main(int args_count, char* args_array[])
{
    VR_Arena_Alloc arena = vr_memory_reserve(16, 1024);

    VR_Socket_UDP socket = vr_socket_udp_reserve((VR_Alloc*) &arena);

    vr_socket_udp_init_bound(socket, VR_Endpoint_IP_Kind_V4, 37134);

    intptr number = 0;

   while (1) {
        VR_Endpoint_IP endpoint = {0};

        char8  message[32]      = {0};
        intptr count            = 0;
        intptr message_number   = 0;

        count = vr_socket_udp_read(socket,
            (uint8*) message, sizeof message, &endpoint);

        printf(INFO " Ricevuto '%.*s'\n", (int) count, message);

        message_number = strtoll(message, NULL, 10);
        number         = intptr_add_safe(number, message_number);

        count = snprintf(message, sizeof message, "%lli", number);

        vr_socket_udp_write(socket, (uint8*) message, count, endpoint);

        printf(INFO " Inviato '%.*s'\n", (int) count, message);
        printf("\n");

        if (message_number == 0) break;
    }

    vr_socket_udp_uninit(socket);

    return 0;
}
