#include <vr_system_memory.h>
#include <vr_system_socket.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INFO "[\x1b[34m  INFO \x1b[0m] "

int main(int args_count, char** args_array)
{
    VR_Arena_Alloc arena = vr_memory_reserve(16, 1024);

    VR_Socket_Tcp listener = vr_socket_tcp_reserve((VR_Alloc*) &arena);
    VR_Socket_Tcp socket   = vr_socket_tcp_reserve((VR_Alloc*) &arena);

    vr_socket_tcp_init_bound(listener, VR_Network_Ip_Addr_Kind_Ver4, 37134);
    vr_socket_tcp_listen(listener);

    while (1) {
        vr_socket_tcp_accept(socket, listener);
        vr_socket_tcp_uninit(socket);
    }

    vr_socket_tcp_uninit(listener);

    return 0;
}
