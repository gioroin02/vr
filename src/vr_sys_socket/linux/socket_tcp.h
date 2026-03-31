#ifndef VR_SYS_SOCKET_LINUX_SOCKET_TCP_H
#define VR_SYS_SOCKET_LINUX_SOCKET_TCP_H

#include "utils.h"

typedef struct
{
    int                handle;
    sockaddr_storage_t address;
}
VR_Linux_Socket_TCP;

VR_Linux_Socket_TCP* vr_linux_socket_tcp_reserve(VR_Alloc alloc);

bool32 vr_linux_socket_tcp_create(VR_Linux_Socket_TCP* self, VR_Endpoint_IP endpoint);

void vr_linux_socket_tcp_destroy(VR_Linux_Socket_TCP* self);

bool32 vr_linux_socket_tcp_bind(VR_Linux_Socket_TCP* self);

bool32 vr_linux_socket_tcp_listen(VR_Linux_Socket_TCP* self);

bool32 vr_linux_socket_tcp_accept(VR_Linux_Socket_TCP* self, VR_Linux_Socket_TCP* listener);

bool32 vr_linux_socket_tcp_connect(VR_Linux_Socket_TCP* self, VR_Endpoint_IP endpoint);

intptr vr_linux_socket_tcp_write(VR_Linux_Socket_TCP* self, uint8* pntr, intptr size);

intptr vr_linux_socket_tcp_read(VR_Linux_Socket_TCP* self, uint8* pntr, intptr size);

VR_Endpoint_IP vr_linux_socket_tcp_get_endpoint(VR_Linux_Socket_TCP* self);

#endif
