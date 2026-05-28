#include <vr_platform_socket.h>

#include <stdio.h>
#include <string.h>

int main(void)
{
    VrUint8 memory[VR_SINT_KILO_2] = {0};

    VrArenaAlloc arena = vr_arena_alloc_make(memory, sizeof memory);

    VrUdpSocket socket = vr_udp_socket_reserve((VrAlloc*) &arena);

    VrAddressIp addr = vr_address_ipv4(
        VR_ADDRESS_IP_VER4_LOCAL, 5000);

    vr_udp_socket_init(socket, vr_address_ipv4_empty());

    VrChar8 message[32] = "Ciao";
    VrSint  count       = strlen(message);

    vr_udp_socket_write(socket, (VrUint8*) message, count, addr);

    printf("[INFO] Inviato '%.*s'\n", count, message);

    count = vr_udp_socket_read(socket,
        (VrUint8*) message, sizeof message, &addr);

    printf("[INFO] Ricevuto '%.*s'\n", count, message);

    vr_udp_socket_uninit(socket);

    return 0;
}
