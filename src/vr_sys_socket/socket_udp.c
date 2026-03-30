#ifndef VR_SYS_SOCKET_SOCKET_UDP_C
#define VR_SYS_SOCKET_SOCKET_UDP_C

#include "socket_udp.h"

#if VR_SYSTEM == VR_SYSTEM_WINDOWS

    #include "win32/export.c"

    #define _VR_Socket_UDP_ VR_Win32_Socket_UDP

    #define _vr_socket_udp_reserve_      vr_win32_socket_udp_reserve
    #define _vr_socket_udp_create_       vr_win32_socket_udp_create
    #define _vr_socket_udp_destroy_      vr_win32_socket_udp_destroy
    #define _vr_socket_udp_bind_         vr_win32_socket_udp_bind
    #define _vr_socket_udp_write_        vr_win32_socket_udp_write
    #define _vr_socket_udp_read_         vr_win32_socket_udp_read
    #define _vr_socket_udp_get_endpoint_ vr_win32_socket_udp_get_endpoint

#elif VR_SYSTEM == VR_SYSTEM_LINUX

    #include "linux/export.c"

    #define _VR_Socket_UDP_ VR_Linux_Socket_UDP

    #define _vr_socket_udp_reserve_      vr_linux_socket_udp_reserve
    #define _vr_socket_udp_create_       vr_linux_socket_udp_create
    #define _vr_socket_udp_destroy_      vr_linux_socket_udp_destroy
    #define _vr_socket_udp_bind_         vr_linux_socket_udp_bind
    #define _vr_socket_udp_write_        vr_linux_socket_udp_write
    #define _vr_socket_udp_read_         vr_linux_socket_udp_read
    #define _vr_socket_udp_get_endpoint_ vr_linux_socket_udp_get_endpoint

#else

    #error "Unknown operating system"

#endif

struct VR_Socket_UDP { void* private; };

VR_Socket_UDP* vr_socket_udp_reserve(VR_Alloc alloc)
{
    return (VR_Socket_UDP*) _vr_socket_udp_reserve_(alloc);
}

bool32 vr_socket_udp_create(VR_Socket_UDP* self, VR_Endpoint_IP endpoint)
{
    return _vr_socket_udp_create_((_VR_Socket_UDP_*) self, endpoint);
}

void vr_socket_udp_destroy(VR_Socket_UDP* self)
{
    return _vr_socket_udp_destroy_((_VR_Socket_UDP_*) self);
}

bool32 vr_socket_udp_bind(VR_Socket_UDP* self)
{
    return _vr_socket_udp_bind_((_VR_Socket_UDP_*) self);
}

intptr vr_socket_udp_write(VR_Socket_UDP* self, uint8* pntr, intptr size, VR_Endpoint_IP endpoint)
{
    return _vr_socket_udp_write_((_VR_Socket_UDP_*) self, pntr, size, endpoint);
}

intptr vr_socket_udp_read(VR_Socket_UDP* self, uint8* pntr, intptr size, VR_Endpoint_IP* endpoint)
{
    return _vr_socket_udp_read_((_VR_Socket_UDP_*) self, pntr, size, endpoint);
}

VR_Endpoint_IP vr_socket_udp_get_endpoint(VR_Socket_UDP* self)
{
    return _vr_socket_udp_get_endpoint_((_VR_Socket_UDP_*) self);
}

#endif
