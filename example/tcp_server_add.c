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

    VrTcpListener listener = vr_tcp_listener_reserve((VrAlloc*) &arena);
    VrTcpSocket   socket   = vr_tcp_socket_reserve((VrAlloc*) &arena);

    vr_tcp_listener_init(listener, vr_address_ipv4_local(50000));
    vr_tcp_listener_bind_and_listen(listener);

    vr_tcp_socket_accept(socket, listener);

    printf(INFO "Sessione iniziata (%lli)\n", 0);

    VrSint number = 0;

    while (1) {
        VrChar8 message[32]    = {0};
        VrSint  count          = 0;
        VrSint  message_number = 0;

        count = vr_tcp_socket_read(socket, (VrUint8*) message, sizeof message);

        printf(INFO "Nuovo messaggio:\n");
        printf("    Ricevuto '%.*s'\n", count, message);

        message_number = strtoll(message, NULL, 10);
        number         = intptr_add_safe(number, message_number);

        count = snprintf(message, sizeof message, "%lli", number);

        vr_tcp_socket_write_all(socket, (VrUint8*) message, count);

        printf("    Inviato '%.*s'\n", count, message);

        if (message_number == 0) break;
    }

    vr_tcp_socket_uninit(socket);
    vr_tcp_listener_uninit(listener);

    return 0;
}
