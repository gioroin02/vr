#ifndef VR_SYS_SOCKET_SOCKET_TCP_C
#define VR_SYS_SOCKET_SOCKET_TCP_C

#include "socket_tcp.h"

#if VR_SYSTEM == VR_SYSTEM_WINDOWS

    #include "win32/export.c"

    #define _VR_Socket_TCP_ VR_Win32_Socket_TCP

    #define _vr_socket_tcp_reserve_      vr_win32_socket_tcp_reserve
    #define _vr_socket_tcp_create_       vr_win32_socket_tcp_create
    #define _vr_socket_tcp_destroy_      vr_win32_socket_tcp_destroy
    #define _vr_socket_tcp_bind_         vr_win32_socket_tcp_bind
    #define _vr_socket_tcp_listen_       vr_win32_socket_tcp_listen
    #define _vr_socket_tcp_accept_       vr_win32_socket_tcp_accept
    #define _vr_socket_tcp_connect_      vr_win32_socket_tcp_connect
    #define _vr_socket_tcp_write_        vr_win32_socket_tcp_write
    #define _vr_socket_tcp_read_         vr_win32_socket_tcp_read
    #define _vr_socket_tcp_get_endpoint_ vr_win32_socket_tcp_get_endpoint

#elif VR_SYSTEM == VR_SYSTEM_LINUX

    #include "linux/export.c"

    #define _VR_Socket_TCP_ VR_Linux_Socket_TCP

    #define _vr_socket_tcp_reserve_      vr_linux_socket_tcp_reserve
    #define _vr_socket_tcp_create_       vr_linux_socket_tcp_create
    #define _vr_socket_tcp_destroy_      vr_linux_socket_tcp_destroy
    #define _vr_socket_tcp_bind_         vr_linux_socket_tcp_bind
    #define _vr_socket_tcp_listen_       vr_linux_socket_tcp_listen
    #define _vr_socket_tcp_accept_       vr_linux_socket_tcp_accept
    #define _vr_socket_tcp_connect_      vr_linux_socket_tcp_connect
    #define _vr_socket_tcp_write_        vr_linux_socket_tcp_write
    #define _vr_socket_tcp_read_         vr_linux_socket_tcp_read
    #define _vr_socket_tcp_get_endpoint_ vr_linux_socket_tcp_get_endpoint

#else

    #error "Unknown operating system"

#endif

struct VR_Socket_TCP { void* private; };

VR_Socket_TCP* vr_socket_tcp_reserve(VR_Alloc alloc)
{
    return (VR_Socket_TCP*) _vr_socket_tcp_reserve_(alloc);
}

bool32 vr_socket_tcp_create(VR_Socket_TCP* self, VR_Endpoint_IP endpoint)
{
    return _vr_socket_tcp_create_((_VR_Socket_TCP_*) self, endpoint);
}

void vr_socket_tcp_destroy(VR_Socket_TCP* self)
{
    return _vr_socket_tcp_destroy_((_VR_Socket_TCP_*) self);
}

bool32 vr_socket_tcp_bind(VR_Socket_TCP* self)
{
    return _vr_socket_tcp_bind_((_VR_Socket_TCP_*) self);
}

bool32 vr_socket_tcp_listen(VR_Socket_TCP* self)
{
    return _vr_socket_tcp_listen_((_VR_Socket_TCP_*) self);
}

bool32 vr_socket_tcp_accept(VR_Socket_TCP* self, VR_Socket_TCP* listener)
{
    return _vr_socket_tcp_accept_((_VR_Socket_TCP_*) self, (_VR_Socket_TCP_*) listener);
}

bool32 vr_socket_tcp_connect(VR_Socket_TCP* self, VR_Endpoint_IP endpoint)
{
    return _vr_socket_tcp_connect_((_VR_Socket_TCP_*) self, endpoint);
}

intptr vr_socket_tcp_write(VR_Socket_TCP* self, uint8* pntr, intptr size)
{
    return _vr_socket_tcp_write_((_VR_Socket_TCP_*) self, pntr, size);
}

intptr vr_socket_tcp_read(VR_Socket_TCP* self, uint8* pntr, intptr size)
{
    return _vr_socket_tcp_read_((_VR_Socket_TCP_*) self, pntr, size);
}

VR_Endpoint_IP vr_socket_tcp_get_endpoint(VR_Socket_TCP* self)
{
    return _vr_socket_tcp_get_endpoint_((_VR_Socket_TCP_*) self);
}

#endif
