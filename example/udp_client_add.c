#include <vr_platform_memory.h>
#include <vr_platform_socket.h>

#include <stdio.h>
#include <stdlib.h>

#define INFO "[\x1b[34m  INFO \x1b[0m] "

int main(int args_count, char** args_array)
{
    VrArenaAlloc arena = vr_memory_reserve(16, 1024);

    VrUdpSocket socket      = vr_udp_socket_reserve((VrAlloc*) &arena);
    VrAddressIp server_addr = vr_address_ip_ver4_local(50000);

    vr_udp_socket_init(socket, vr_address_ip_ver4_empty());

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

        vr_udp_socket_write(socket, (VrUint8*) message, count, server_addr);

        printf(INFO "Nuovo messaggio:\n");
        printf("    Inviato '%.*s'\n", count, message);

        VrAddressIp addr    = {0};
        VrBool32    success = 0;

        while (success == 0) {
            count = vr_udp_socket_read(socket, (VrUint8*) message, sizeof message, &addr);

            if (vr_address_ip_is_equal(server_addr, addr) == 0)
                continue;

            printf("    Ricevuto '%.*s'\n", count, message);

            success = 1;
        }

        if (message_number == 0) break;
    }

    vr_udp_socket_uninit(socket);

    return 0;
}
