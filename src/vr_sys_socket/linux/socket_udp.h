#ifndef VR_SYS_SOCKET_LINUX_SOCKET_UDP_H
#define VR_SYS_SOCKET_LINUX_SOCKET_UDP_H

#include "utils.h"

typedef struct
{
    int                handle;
    sockaddr_storage_t address;
}
VR_Linux_Socket_UDP;

VR_Linux_Socket_UDP* vr_linux_socket_udp_reserve(VR_Alloc alloc);

bool32 vr_linux_socket_udp_create(VR_Linux_Socket_UDP* self, VR_Endpoint_IP endpoint);

void vr_linux_socket_udp_destroy(VR_Linux_Socket_UDP* self);

bool32 vr_linux_socket_udp_bind(VR_Linux_Socket_UDP* self);

intptr vr_linux_socket_udp_write(VR_Linux_Socket_UDP* self, uint8* pntr, intptr size, VR_Endpoint_IP endpoint);

intptr vr_linux_socket_udp_read(VR_Linux_Socket_UDP* self, uint8* pntr, intptr size, VR_Endpoint_IP* endpoint);

VR_Endpoint_IP vr_linux_socket_udp_get_endpoint(VR_Linux_Socket_UDP* self);

#endif
