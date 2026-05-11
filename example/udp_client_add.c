#include <vr_system_memory.h>
#include <vr_system_socket.h>

#include <stdio.h>
#include <stdlib.h>

#define INFO "[\x1b[34m  INFO \x1b[0m]"

int main(int args_count, char* args_array[])
{
    VR_Arena_Alloc arena = vr_memory_reserve(16, 1024);

    VR_Socket_UDP  socket = vr_socket_udp_reserve((VR_Alloc*) &arena);
    VR_Endpoint_IP server = vr_endpoint_ip_ver4(VR_ENDPOINT_IPV4_LOCAL, 37134);

    vr_socket_udp_init(socket, server.kind);

    while (1) {
        char8  message[32]      = {0};
        intptr count            = 0;
        char8* message_end_pntr = message;
        intptr message_number   = 0;

        while (message == message_end_pntr) {
            printf("$ ");

            fgets(message, sizeof message - 1, stdin);

            while (message[count] != 0) count += 1;

            message_number = strtoll(message,
                &message_end_pntr, 10);
        }

        count = snprintf(message, sizeof message, "%lli", message_number);

        vr_socket_udp_write(socket, (uint8*) message, count, server);

        printf(INFO " Inviato '%.*s'\n", (int) count, message);

        VR_Endpoint_IP endpoint = {0};
        bool32         success  = 0;

        while (success == 0) {
            count = vr_socket_udp_read(socket,
                (uint8*) message, sizeof message, &endpoint);

            if (vr_endpoint_ip_is_equal(server, endpoint) == 0)
                continue;

            printf(INFO " Ricevuto '%.*s'\n", (int) count, message);

            success = 1;
        }

        if (message_number == 0) break;
    }

    vr_socket_udp_uninit(socket);

    return 0;
}
