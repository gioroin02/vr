#include <vr_system_socket.h>
#include <stdio.h>

int main(int args_count, char* args_array[])
{
    uint8 memory[VR_INTPTR_KIBI] = {0};

    VR_Arena_Alloc arena = vr_arena_alloc_make(memory, sizeof memory);
    VR_Alloc       alloc = vr_alloc_arena(&arena);

    VR_Socket_UDP  socket   = vr_socket_udp_reserve(alloc);
    VR_Endpoint_IP endpoint = vr_endpoint_ip_none();

    vr_socket_udp_init_bound(socket, VR_Endpoint_IP_Kind_V4, 5000);

    char message[32] = {0};

    intptr count = vr_socket_udp_read(socket,
        (uint8*) message, sizeof message, &endpoint);

    printf("[INFO] Ricevuto '%.*s'\n", (int) count, message);

    vr_socket_udp_write(socket, (uint8*) message, count, endpoint);

    printf("[INFO] Inviato '%.*s'\n", (int) count, message);

    vr_socket_udp_deinit(socket);

    return 0;
}
