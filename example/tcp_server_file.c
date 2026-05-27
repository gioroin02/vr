#include <vr_platform_memory.h>
#include <vr_platform_socket.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INFO "[\x1b[34m  INFO \x1b[0m] "

int main(int args_count, char** args_array)
{
    VrArenaAlloc arena = vr_memory_reserve(16, 1024);

    VrTcpListener listener = vr_tcp_listener_reserve((VrAlloc*) &arena);
    VrTcpSocket   socket   = vr_tcp_socket_reserve((VrAlloc*) &arena);

    vr_tcp_listener_init(listener, vr_address_ip_ver4_local(50000));
    vr_tcp_listener_bind_and_listen(listener);

    while (1) {
        vr_tcp_socket_accept(socket, listener);
        vr_tcp_socket_uninit(socket);
    }

    vr_tcp_listener_uninit(listener);

    return 0;
}
