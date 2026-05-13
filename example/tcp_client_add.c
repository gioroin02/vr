#include <vr_system_memory.h>
#include <vr_system_socket.h>

#include <stdio.h>
#include <stdlib.h>

#define INFO "[\x1b[34m  INFO \x1b[0m] "

int main(int args_count, char* args_array[])
{
    VR_ArenaAlloc arena = vr_memory_reserve(16, 1024);

    VR_SocketTcp socket = vr_socket_tcp_reserve((VR_Alloc*) &arena);

    VR_NetworkIpAddr server_addr = vr_network_ip_addr_ver4(
        VR_NETWORK_IP_ADDR_VER4_LOCAL, 37134);

    vr_socket_tcp_init(socket, server_addr.kind);
    vr_socket_tcp_connect(socket, server_addr);

    printf(INFO "Sessione iniziata\n");

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

        vr_socket_tcp_write_all(socket, (uint8*) message, count);

        printf(INFO "Nuovo messaggio:\n");
        printf("    Inviato '%.*s'\n", count, message);

        count = vr_socket_tcp_read(socket, (uint8*) message, sizeof message);

        printf("    Ricevuto '%.*s'\n", count, message);

        if (message_number == 0) break;
    }

    vr_socket_tcp_uninit(socket);

    return 0;
}
