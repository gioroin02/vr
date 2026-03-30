#ifndef VR_SYS_SOCKET_WIN32_SOCKET_UDP_H
#define VR_SYS_SOCKET_WIN32_SOCKET_UDP_H

#include "utils.h"

typedef struct
{
    SOCKET             handle;
    sockaddr_storage_t address;
}
VR_Win32_Socket_UDP;

VR_Win32_Socket_UDP* vr_win32_socket_udp_reserve(VR_Alloc alloc);

bool32 vr_win32_socket_udp_create(VR_Win32_Socket_UDP* self, VR_Endpoint_IP endpoint);

void vr_win32_socket_udp_destroy(VR_Win32_Socket_UDP* self);

bool32 vr_win32_socket_udp_bind(VR_Win32_Socket_UDP* self);

intptr vr_win32_socket_udp_write(VR_Win32_Socket_UDP* self, uint8* pntr, intptr size, VR_Endpoint_IP endpoint);

intptr vr_win32_socket_udp_read(VR_Win32_Socket_UDP* self, uint8* pntr, intptr size, VR_Endpoint_IP* endpoint);

VR_Endpoint_IP vr_win32_socket_udp_get_endpoint(VR_Win32_Socket_UDP* self);

#endif
