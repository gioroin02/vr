#include <vr_platform_socket.h>

#include <stdio.h>

int main(void)
{
    VrUint8 memory[VR_SINT_KILO_2] = {0};

    VrArenaAlloc arena = vr_arena_alloc_make(memory, sizeof memory);

    VrTcpListener listener = vr_tcp_listener_reserve((VrAlloc*) &arena);
    VrTcpSocket   socket   = vr_tcp_socket_reserve((VrAlloc*) &arena);

    vr_tcp_listener_init(listener, vr_address_ip_ver4_local(5000));
    vr_tcp_listener_bind(listener);

    vr_tcp_socket_accept(socket, listener);

    VrChar8 message[32] = {0};

    VrSint count = vr_tcp_socket_read(socket, (VrUint8*) message, sizeof message);

    printf("[INFO] Ricevuto '%.*s'\n", count, message);

    vr_tcp_socket_write(socket, (VrUint8*) message, count);

    printf("[INFO] Inviato '%.*s'\n", count, message);

    vr_tcp_socket_uninit(socket);
    vr_tcp_listener_uninit(listener);

    return 0;
}
