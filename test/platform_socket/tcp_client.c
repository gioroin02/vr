#include <vr_platform_socket.h>

#include <stdio.h>
#include <string.h>

int main(void)
{
    VrUint8 memory[VR_SINT_KILO_2] = {0};

    VrArenaAlloc arena = vr_arena_alloc_make(memory, sizeof memory);

    VrTcpSocket socket = vr_tcp_socket_reserve((VrAlloc*) &arena);

    vr_tcp_socket_init(socket, vr_address_ipv4_empty());
    vr_tcp_socket_connect(socket, vr_address_ipv4_local(5000));

    VrUint8 msg_buffer[32] = "Ciao!";
    VrSint  msg_size       = sizeof msg_buffer;
    VrSint  msg_count      = strlen((VrChar8*) msg_buffer);

    vr_tcp_socket_write_all(socket, msg_buffer, msg_count);

    printf("[INFO] Inviato '%.*s'\n", msg_count, msg_buffer);

    msg_count = vr_tcp_socket_read(socket, msg_buffer, msg_size);

    printf("[INFO] Ricevuto '%.*s'\n", msg_count, msg_buffer);

    vr_tcp_socket_uninit(socket);

    return 0;
}
