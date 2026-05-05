#ifndef VR_SYS_SOCKET_C
#define VR_SYS_SOCKET_C

#include "vr_sys_socket.h"

VR_Endpoint_IP vr_endpoint_ip_none()
{
    return (VR_Endpoint_IP) {.kind = VR_Endpoint_IP_Kind_None};
}

VR_Endpoint_IP vr_endpoint_ip_empty(VR_Endpoint_IP_Kind kind)
{
    switch (kind) {
        case VR_Endpoint_IP_Kind_4: { return (VR_Endpoint_IP) {.kind = kind}; } break;
        case VR_Endpoint_IP_Kind_6: { return (VR_Endpoint_IP) {.kind = kind}; } break;

        default: break;
    }

    return vr_endpoint_ip_none();
}

VR_Endpoint_IP vr_endpoint_ip_local(VR_Endpoint_IP_Kind kind, uint16 port)
{
    switch (kind) {
        case VR_Endpoint_IP_Kind_4: {
            return (VR_Endpoint_IP) {
                .kind = kind,
                .port = port,
                .ip_4 = (VR_Endpoint_IP4) {.elem_0 = 0x7F, .elem_3 = 0x01},
            };
        } break;

        case VR_Endpoint_IP_Kind_6: {
            return (VR_Endpoint_IP) {
                .kind = kind,
                .port = port,
                .ip_6 = (VR_Endpoint_IP6) {.elem_15 = 0x01},
            };
        } break;

        default: break;
    }

    return vr_endpoint_ip_none();
}

bool32 vr_endpoint_ip_is_equal(VR_Endpoint_IP self, VR_Endpoint_IP other)
{
    if (self.kind != other.kind || self.port != other.port)
        return 0;

    switch (self.kind) {
        case VR_Endpoint_IP_Kind_4: {
            for (intptr i = 0; i < VR_ENDPOINT_IP4_SIZE; i += 1) {
                if (self.ip_4.elems[i] != other.ip_4.elems[i])
                    return 0;
            }

            return 1;
        } break;

        case VR_Endpoint_IP_Kind_6: {
            for (intptr i = 0; i < VR_ENDPOINT_IP6_SIZE; i += 1) {
                if (self.ip_6.elems[i] != other.ip_6.elems[i])
                    return 0;
            }

            return 1;
        } break;

        default: break;
    }

    return 0;
}

#if VR_SYSTEM == VR_SYSTEM_WINDOWS

    #include "vr_sys_socket_win32.c"

    #define _VR_Socket_TCP_ VR_Win32_Socket_TCP

    #define _vr_socket_tcp_reserve_  vr_win32_socket_tcp_reserve
    #define _vr_socket_tcp_create_   vr_win32_socket_tcp_create
    #define _vr_socket_tcp_destroy_  vr_win32_socket_tcp_destroy
    #define _vr_socket_tcp_bind_     vr_win32_socket_tcp_bind
    #define _vr_socket_tcp_listen_   vr_win32_socket_tcp_listen
    #define _vr_socket_tcp_accept_   vr_win32_socket_tcp_accept
    #define _vr_socket_tcp_connect_  vr_win32_socket_tcp_connect
    #define _vr_socket_tcp_write_    vr_win32_socket_tcp_write
    #define _vr_socket_tcp_read_     vr_win32_socket_tcp_read
    #define _vr_socket_tcp_endpoint_ vr_win32_socket_tcp_endpoint

    #define _VR_Socket_UDP_ VR_Win32_Socket_UDP

    #define _vr_socket_udp_reserve_  vr_win32_socket_udp_reserve
    #define _vr_socket_udp_create_   vr_win32_socket_udp_create
    #define _vr_socket_udp_destroy_  vr_win32_socket_udp_destroy
    #define _vr_socket_udp_bind_     vr_win32_socket_udp_bind
    #define _vr_socket_udp_write_    vr_win32_socket_udp_write
    #define _vr_socket_udp_read_     vr_win32_socket_udp_read
    #define _vr_socket_udp_endpoint_ vr_win32_socket_udp_endpoint

#elif VR_SYSTEM == VR_SYSTEM_LINUX

    #include "vr_sys_socket_linux.c"

    #define _VR_Socket_TCP_ VR_Linux_Socket_TCP

    #define _vr_socket_tcp_reserve_  vr_linux_socket_tcp_reserve
    #define _vr_socket_tcp_create_   vr_linux_socket_tcp_create
    #define _vr_socket_tcp_destroy_  vr_linux_socket_tcp_destroy
    #define _vr_socket_tcp_bind_     vr_linux_socket_tcp_bind
    #define _vr_socket_tcp_listen_   vr_linux_socket_tcp_listen
    #define _vr_socket_tcp_accept_   vr_linux_socket_tcp_accept
    #define _vr_socket_tcp_connect_  vr_linux_socket_tcp_connect
    #define _vr_socket_tcp_write_    vr_linux_socket_tcp_write
    #define _vr_socket_tcp_read_     vr_linux_socket_tcp_read
    #define _vr_socket_tcp_endpoint_ vr_linux_socket_tcp_endpoint

    #define _VR_Socket_UDP_ VR_Linux_Socket_UDP

    #define _vr_socket_udp_reserve_  vr_linux_socket_udp_reserve
    #define _vr_socket_udp_create_   vr_linux_socket_udp_create
    #define _vr_socket_udp_destroy_  vr_linux_socket_udp_destroy
    #define _vr_socket_udp_bind_     vr_linux_socket_udp_bind
    #define _vr_socket_udp_write_    vr_linux_socket_udp_write
    #define _vr_socket_udp_read_     vr_linux_socket_udp_read
    #define _vr_socket_udp_endpoint_ vr_linux_socket_udp_endpoint

#else

    #error "Unknown platform"

#endif

struct VR_Socket_TCP { void* private; };

VR_Socket_TCP* vr_socket_tcp_reserve(VR_Alloc alloc)
{
    return (VR_Socket_TCP*) _vr_socket_tcp_reserve_(alloc);
}

bool32 vr_socket_tcp_create(VR_Socket_TCP* self, VR_Endpoint_IP_Kind kind)
{
    return _vr_socket_tcp_create_((_VR_Socket_TCP_*) self, vr_endpoint_ip_empty(kind));
}

bool32 vr_socket_tcp_create_bound(VR_Socket_TCP* self, VR_Endpoint_IP_Kind kind, uint16 port)
{
    VR_Endpoint_IP endpoint = vr_endpoint_ip_local(kind, port);
    bool32         status   = 1;

    if (port == 0) return 0;

    status &= _vr_socket_tcp_create_((_VR_Socket_TCP_*) self, endpoint);
    status &= _vr_socket_tcp_bind_((_VR_Socket_TCP_*) self);

    if (status == 0)
        _vr_socket_tcp_destroy_((_VR_Socket_TCP_*) self);

    return status;
}

void vr_socket_tcp_destroy(VR_Socket_TCP* self)
{
    return _vr_socket_tcp_destroy_((_VR_Socket_TCP_*) self);
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

VR_Endpoint_IP vr_socket_tcp_endpoint(VR_Socket_TCP* self)
{
    return _vr_socket_tcp_endpoint_((_VR_Socket_TCP_*) self);
}

struct VR_Socket_UDP { void* private; };

VR_Socket_UDP* vr_socket_udp_reserve(VR_Alloc alloc)
{
    return (VR_Socket_UDP*) _vr_socket_udp_reserve_(alloc);
}

bool32 vr_socket_udp_create(VR_Socket_UDP* self, VR_Endpoint_IP_Kind kind)
{
    return _vr_socket_udp_create_((_VR_Socket_UDP_*) self, vr_endpoint_ip_empty(kind));
}

bool32 vr_socket_udp_create_bound(VR_Socket_UDP* self, VR_Endpoint_IP_Kind kind, uint16 port)
{
    VR_Endpoint_IP endpoint = vr_endpoint_ip_local(kind, port);
    bool32         status   = 1;

    if (port == 0) return 0;

    status &= _vr_socket_udp_create_((_VR_Socket_UDP_*) self, endpoint);
    status &= _vr_socket_udp_bind_((_VR_Socket_UDP_*) self);

    if (status == 0)
        _vr_socket_udp_destroy_((_VR_Socket_UDP_*) self);

    return status;
}

void vr_socket_udp_destroy(VR_Socket_UDP* self)
{
    return _vr_socket_udp_destroy_((_VR_Socket_UDP_*) self);
}

intptr vr_socket_udp_write(VR_Socket_UDP* self, uint8* pntr, intptr size, VR_Endpoint_IP endpoint)
{
    return _vr_socket_udp_write_((_VR_Socket_UDP_*) self, pntr, size, endpoint);
}

intptr vr_socket_udp_read(VR_Socket_UDP* self, uint8* pntr, intptr size, VR_Endpoint_IP* endpoint)
{
    return _vr_socket_udp_read_((_VR_Socket_UDP_*) self, pntr, size, endpoint);
}

VR_Endpoint_IP vr_socket_udp_endpoint(VR_Socket_UDP* self)
{
    return _vr_socket_udp_endpoint_((_VR_Socket_UDP_*) self);
}

#endif
