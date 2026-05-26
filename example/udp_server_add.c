#include <vr_platform_memory.h>
#include <vr_platform_socket.h>

#include <stdio.h>
#include <stdlib.h>

VrSint intptr_add_safe(VrSint a, VrSint b)
{
    if (b <= 0 || a > VR_SINT_MAX - b)
        return a;

    return a + b;
}

#define INFO "[\x1b[34m  INFO \x1b[0m] "

int main(int args_count, char** args_array)
{
    VrArenaAlloc arena = vr_memory_reserve(16, 1024);

    VrUdpSocket socket = vr_udp_socket_reserve((VrAlloc*) &arena);

    vr_udp_socket_init(socket, vr_address_ip_ver4_local(37134));
    vr_udp_socket_bind(socket);

    VrSint number = 0;

   while (1) {
        VrAddressIp addr           = {0};
        VrChar8     message[32]    = {0};
        VrSint      count          = 0;
        VrSint      message_number = 0;

        count = vr_udp_socket_read(socket, (VrUint8*) message, sizeof message, &addr);

        printf(INFO "Nuovo messaggio:\n");
        printf("    Ricevuto '%.*s'\n", count, message);

        message_number = strtoll(message, NULL, 10);
        number         = intptr_add_safe(number, message_number);

        count = snprintf(message, sizeof message, "%lli", number);

        vr_udp_socket_write_all(socket, (VrUint8*) message, count, addr);

        printf("    Inviato '%.*s'\n", count, message);

        if (message_number == 0) break;
    }

    vr_udp_socket_uninit(socket);

    return 0;
}
