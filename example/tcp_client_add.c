#include <vr_platform_memory.h>
#include <vr_platform_socket.h>

#include <stdio.h>
#include <stdlib.h>

#define INFO "[\x1b[34m  INFO \x1b[0m] "

int main(int args_count, char** args_array)
{
    VrArenaAlloc arena = vr_memory_reserve(16, 1024);

    VrTcpSocket socket = vr_tcp_socket_reserve((VrAlloc*) &arena);

    vr_tcp_socket_init(socket, vr_address_ipv4_empty());
    vr_tcp_socket_connect(socket, vr_address_ipv4_local(50000));

    printf(INFO "Sessione iniziata\n");

    while (1) {
        VrChar8  message[32]      = {0};
        VrSint   count            = 0;
        VrChar8* message_end_pntr = message;
        VrSint   message_number   = 0;

        while (message == message_end_pntr) {
            printf("$ ");

            fgets(message, sizeof message - 1, stdin);

            while (message[count] != 0) count += 1;

            message_number = strtoll(message,
                &message_end_pntr, 10);
        }

        count = snprintf(message, sizeof message, "%lli", message_number);

        vr_tcp_socket_write_all(socket, (VrUint8*) message, count);

        printf(INFO "Nuovo messaggio:\n");
        printf("    Inviato '%.*s'\n", count, message);

        count = vr_tcp_socket_read(socket, (VrUint8*) message, sizeof message);

        printf("    Ricevuto '%.*s'\n", count, message);

        if (message_number == 0) break;
    }

    vr_tcp_socket_uninit(socket);

    return 0;
}
