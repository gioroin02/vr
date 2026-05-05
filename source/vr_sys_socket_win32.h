#ifndef VR_SYS_SOCKET_WIN32_H
#define VR_SYS_SOCKET_WIN32_H

#include "vr_sys_socket.h"

#define WIN32_LEAN_AND_MEAN
#define NOGDI
#include <windows.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>

typedef struct sockaddr_storage sockaddr_storage_t;
typedef struct sockaddr         sockaddr_t;
typedef struct sockaddr_in      sockaddr_ip4_t;
typedef struct sockaddr_in6     sockaddr_ip6_t;

bool32 vr_win32_socket_start();

void vr_win32_socket_stop();

sockaddr_storage_t vr_win32_sockaddr_make(VR_Endpoint_IP endpoint);

sockaddr_storage_t vr_win32_sockaddr_make_any(VR_Endpoint_IP_Kind kind, uint16 port);

intptr vr_win32_sockaddr_size(sockaddr_storage_t* self);

VR_Endpoint_IP vr_win32_sockaddr_endpoint(sockaddr_storage_t* self);

typedef struct
{
    SOCKET             handle;
    sockaddr_storage_t address;
    void*              queue;
}
VR_Win32_Socket_TCP;

VR_Win32_Socket_TCP* vr_win32_socket_tcp_reserve(VR_Alloc alloc);

bool32 vr_win32_socket_tcp_create(VR_Win32_Socket_TCP* self, VR_Endpoint_IP endpoint);

void vr_win32_socket_tcp_destroy(VR_Win32_Socket_TCP* self);

bool32 vr_win32_socket_tcp_bind(VR_Win32_Socket_TCP* self);

bool32 vr_win32_socket_tcp_listen(VR_Win32_Socket_TCP* self);

bool32 vr_win32_socket_tcp_accept(VR_Win32_Socket_TCP* self, VR_Win32_Socket_TCP* listener);

bool32 vr_win32_socket_tcp_connect(VR_Win32_Socket_TCP* self, VR_Endpoint_IP endpoint);

intptr vr_win32_socket_tcp_write(VR_Win32_Socket_TCP* self, uint8* pntr, intptr size);

intptr vr_win32_socket_tcp_read(VR_Win32_Socket_TCP* self, uint8* pntr, intptr size);

VR_Endpoint_IP vr_win32_socket_tcp_endpoint(VR_Win32_Socket_TCP* self);

typedef struct
{
    SOCKET             handle;
    sockaddr_storage_t address;
    void*              queue;
}
VR_Win32_Socket_UDP;

VR_Win32_Socket_UDP* vr_win32_socket_udp_reserve(VR_Alloc alloc);

bool32 vr_win32_socket_udp_create(VR_Win32_Socket_UDP* self, VR_Endpoint_IP endpoint);

void vr_win32_socket_udp_destroy(VR_Win32_Socket_UDP* self);

bool32 vr_win32_socket_udp_bind(VR_Win32_Socket_UDP* self);

intptr vr_win32_socket_udp_write(VR_Win32_Socket_UDP* self, uint8* pntr, intptr size, VR_Endpoint_IP endpoint);

intptr vr_win32_socket_udp_read(VR_Win32_Socket_UDP* self, uint8* pntr, intptr size, VR_Endpoint_IP* endpoint);

VR_Endpoint_IP vr_win32_socket_udp_endpoint(VR_Win32_Socket_UDP* self);

#endif
