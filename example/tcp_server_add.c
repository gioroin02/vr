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

#define INFO "[\x1b[34m  INFO \x1b[0m] "

int main(int args_count, char* args_array[])
{
    VR_Arena_Alloc arena = vr_memory_reserve(16, 1024);

    VR_Socket_Tcp listener = vr_socket_tcp_reserve((VR_Alloc*) &arena);
    VR_Socket_Tcp socket   = vr_socket_tcp_reserve((VR_Alloc*) &arena);

    vr_socket_tcp_init_bound(listener, VR_Network_Ip_Addr_Kind_Ver4, 37134);
    vr_socket_tcp_listen(listener);

    vr_socket_tcp_accept(socket, listener);

    printf(INFO "Sessione iniziata (%lli)\n", 0);

    intptr number = 0;

    while (1) {
        char8  message[32]    = {0};
        intptr count          = 0;
        intptr message_number = 0;

        count = vr_socket_tcp_read(socket, (uint8*) message, sizeof message);

        printf(INFO "Nuovo messaggio:\n");
        printf("    Ricevuto '%.*s'\n", count, message);

        message_number = strtoll(message, NULL, 10);
        number         = intptr_add_safe(number, message_number);

        count = snprintf(message, sizeof message, "%lli", number);

        vr_socket_tcp_write_all(socket, (uint8*) message, count);

        printf("    Inviato '%.*s'\n", count, message);

        if (message_number == 0) break;
    }

    vr_socket_tcp_uninit(socket);
    vr_socket_tcp_uninit(listener);

    return 0;
}
