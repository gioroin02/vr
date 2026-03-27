#ifndef VR_SYS_SOCKET_WIN32_SOCKET_TCP_H
#define VR_SYS_SOCKET_WIN32_SOCKET_TCP_H

#include "utils.h"

typedef struct
{
    SOCKET             handle;
    sockaddr_storage_t address;
}
VR_Win32_Socket_TCP;

VR_Win32_Socket_TCP* vr_win32_socket_tcp_reserve(VR_Alloc alloc);

bool32 vr_win32_socket_tcp_create(VR_Win32_Socket_TCP* self, VR_Endpoint_IP endpoint);

void vr_win32_socket_tcp_destroy(VR_Win32_Socket_TCP* self);

bool32 vr_win32_socket_tcp_bind(VR_Win32_Socket_TCP* self);

bool32 vr_win32_socket_tcp_listen(VR_Win32_Socket_TCP* self);

bool32 vr_win32_socket_tcp_accept(VR_Win32_Socket_TCP* self, VR_Win32_Socket_TCP* listener);

bool32 vr_win32_socket_tcp_connect(VR_Win32_Socket_TCP* self, VR_Endpoint_IP endpoint);

intptr vr_win32_socket_tcp_write(VR_Win32_Socket_TCP* self, uint8* memory, intptr size);

intptr vr_win32_socket_tcp_read(VR_Win32_Socket_TCP* self, uint8* memory, intptr size);

VR_Endpoint_IP vr_win32_socket_tcp_get_endpoint(VR_Win32_Socket_TCP* self);

#endif
