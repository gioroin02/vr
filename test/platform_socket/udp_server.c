#include <vr_platform_socket.h>

#include <stdio.h>

int main(void)
{
    VrUint8 memory[VR_SINT_KILO_2] = {0};

    VrArenaAlloc arena = vr_arena_alloc_make(memory, sizeof memory);

    VrUdpSocket socket = vr_udp_socket_reserve((VrAlloc*) &arena);
    VrAddressIp addr   = {0};

    vr_udp_socket_init(socket, vr_address_ip_ver4_local(5000));
    vr_udp_socket_bind(socket);

    VrChar8 message[32] = {0};

    VrSint count = vr_udp_socket_read(socket,
        (VrUint8*) message, sizeof message, &addr);

    printf("[INFO] Ricevuto '%.*s'\n", count, message);

    vr_udp_socket_write(socket, (VrUint8*) message, count, addr);

    printf("[INFO] Inviato '%.*s'\n", count, message);

    vr_udp_socket_uninit(socket);

    return 0;
}
