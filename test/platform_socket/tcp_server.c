#include <vr_platform_socket.h>

#include <stdio.h>

int main(void)
{
    VrUint8 memory[VR_SINT_KILO_2] = {0};

    VrArenaAlloc arena = vr_arena_alloc_make(memory, sizeof memory);

    VrTcpListener listener = vr_tcp_listener_reserve((VrAlloc*) &arena);
    VrTcpSocket   socket   = vr_tcp_socket_reserve((VrAlloc*) &arena);

    vr_tcp_listener_init(listener, vr_address_ipv4_local(5000));
    vr_tcp_listener_bind_and_listen(listener);

    vr_tcp_socket_accept(socket, listener);

    VrUint8 msg_buffer[32] = {0};
    VrSint  msg_size       = sizeof msg_buffer;
    VrSint  msg_count      = 0;

    msg_count = vr_tcp_socket_read(socket, msg_buffer, msg_size);

    printf("[INFO] Ricevuto '%.*s'\n", msg_count, msg_buffer);

    vr_tcp_socket_write_all(socket, msg_buffer, msg_count);

    printf("[INFO] Inviato '%.*s'\n", msg_count, msg_buffer);

    vr_tcp_socket_uninit(socket);
    vr_tcp_listener_uninit(listener);

    return 0;
}
