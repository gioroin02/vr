#ifndef VR_SYS_SOCKET_WIN32_SOCKET_UDP_C
#define VR_SYS_SOCKET_WIN32_SOCKET_UDP_C

#include "socket_udp.h"

VR_Win32_Socket_UDP* vr_win32_socket_udp_reserve(VR_Alloc alloc)
{
    return vr_alloc_reserve_of(alloc, 1, VR_Win32_Socket_UDP);
}

bool32 vr_win32_socket_udp_create(VR_Win32_Socket_UDP* self, VR_Endpoint_IP endpoint)
{
    self->handle  = INVALID_SOCKET;
    self->address = (sockaddr_storage_t) {0};

    sockaddr_storage_t address = vr_win32_sockaddr_make(endpoint);
    socklen_t          length  = vr_win32_sockaddr_get_size(&address);

    if (length <= 0 || vr_win32_socket_start() == 0) return 0;

    int flag_handle = WSA_FLAG_OVERLAPPED;
    int flag_ops    = SOCK_DGRAM;

    SOCKET handle = WSASocketW(address.ss_family, flag_ops,
        IPPROTO_UDP, 0, 0, flag_handle);

    if (handle != INVALID_SOCKET) {
        self->handle  = handle;
        self->address = address;

        return 1;
    }

    vr_win32_socket_stop();

    return 0;
}

void vr_win32_socket_udp_destroy(VR_Win32_Socket_UDP* self)
{
    if (self->handle == INVALID_SOCKET) return;

    closesocket(self->handle);

    vr_win32_socket_stop();

    self->handle  = INVALID_SOCKET;
    self->address = (sockaddr_storage_t) {0};
}

bool32 vr_win32_socket_udp_bind(VR_Win32_Socket_UDP* self)
{
    sockaddr_storage_t address = self->address;
    socklen_t          length  = vr_win32_sockaddr_get_size(&address);

    if (length <= 0) return 0;

    if (bind(self->handle, (sockaddr_t*) &address, length) == SOCKET_ERROR)
        return 0;

    return 1;
}

intptr vr_win32_socket_udp_write(VR_Win32_Socket_UDP* self, uint8* pntr, intptr size, VR_Endpoint_IP endpoint)
{
    if (pntr == NULL || size <= 0) return 0;

    sockaddr_storage_t address = vr_win32_sockaddr_make(endpoint);
    socklen_t          length  = vr_win32_sockaddr_get_size(&address);

    intptr result = 0;
    int    count  = 0;

    for (NULL; result < size; result += count) {
        count = sendto(self->handle, (char*) pntr,
            (int) size, 0, (sockaddr_t*) &address, length);

        if (count <= 0 || count > size) break;

        pntr += count;
        size -= count;
    }

    return result;
}

intptr vr_win32_socket_udp_read(VR_Win32_Socket_UDP* self, uint8* pntr, intptr size, VR_Endpoint_IP* endpoint)
{
    if (pntr == NULL || size <= 0) return 0;

    sockaddr_storage_t address = {0};
    socklen_t          length  = sizeof address;

    int count = recvfrom(self->handle, (char*) pntr,
        (int) size, 0, (sockaddr_t*) &address, &length);

    if (count <= 0 || count > size) return 0;

    if (endpoint != NULL)
        *endpoint = vr_win32_sockaddr_get_endpoint(&address);

    return (intptr) count;
}

VR_Endpoint_IP vr_win32_socket_udp_get_endpoint(VR_Win32_Socket_UDP* self)
{
    return vr_win32_sockaddr_get_endpoint(&self->address);
}

#endif
