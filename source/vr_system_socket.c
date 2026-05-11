#ifndef VR_SYSTEM_SOCKET_C
#define VR_SYSTEM_SOCKET_C

#include "vr_system_socket.h"

#if VR_SYSTEM == VR_SYSTEM_WINDOWS

    #include "vr_win32_socket.c"

    #define _vr_socket_tcp_reserve_  vr_win32_socket_tcp_reserve
    #define _vr_socket_tcp_init_     vr_win32_socket_tcp_init
    #define _vr_socket_tcp_uninit_   vr_win32_socket_tcp_uninit
    #define _vr_socket_tcp_bind_     vr_win32_socket_tcp_bind
    #define _vr_socket_tcp_listen_   vr_win32_socket_tcp_listen
    #define _vr_socket_tcp_accept_   vr_win32_socket_tcp_accept
    #define _vr_socket_tcp_connect_  vr_win32_socket_tcp_connect
    #define _vr_socket_tcp_write_    vr_win32_socket_tcp_write
    #define _vr_socket_tcp_read_     vr_win32_socket_tcp_read
    #define _vr_socket_tcp_endpoint_ vr_win32_socket_tcp_endpoint

    #define _vr_socket_udp_reserve_  vr_win32_socket_udp_reserve
    #define _vr_socket_udp_init_     vr_win32_socket_udp_init
    #define _vr_socket_udp_uninit_   vr_win32_socket_udp_uninit
    #define _vr_socket_udp_bind_     vr_win32_socket_udp_bind
    #define _vr_socket_udp_write_    vr_win32_socket_udp_write
    #define _vr_socket_udp_read_     vr_win32_socket_udp_read
    #define _vr_socket_udp_endpoint_ vr_win32_socket_udp_endpoint

#elif VR_SYSTEM == VR_SYSTEM_LINUX

    #include "vr_linux_socket.c"

    #define _vr_socket_tcp_reserve_  vr_linux_socket_tcp_reserve
    #define _vr_socket_tcp_init_     vr_linux_socket_tcp_init
    #define _vr_socket_tcp_uninit_   vr_linux_socket_tcp_uninit
    #define _vr_socket_tcp_bind_     vr_linux_socket_tcp_bind
    #define _vr_socket_tcp_listen_   vr_linux_socket_tcp_listen
    #define _vr_socket_tcp_accept_   vr_linux_socket_tcp_accept
    #define _vr_socket_tcp_connect_  vr_linux_socket_tcp_connect
    #define _vr_socket_tcp_write_    vr_linux_socket_tcp_write
    #define _vr_socket_tcp_read_     vr_linux_socket_tcp_read
    #define _vr_socket_tcp_endpoint_ vr_linux_socket_tcp_endpoint

    #define _vr_socket_udp_reserve_  vr_linux_socket_udp_reserve
    #define _vr_socket_udp_init_     vr_linux_socket_udp_init
    #define _vr_socket_udp_uninit_   vr_linux_socket_udp_uninit
    #define _vr_socket_udp_bind_     vr_linux_socket_udp_bind
    #define _vr_socket_udp_write_    vr_linux_socket_udp_write
    #define _vr_socket_udp_read_     vr_linux_socket_udp_read
    #define _vr_socket_udp_endpoint_ vr_linux_socket_udp_endpoint

#else

    #error "Unknown platform"

#endif

VR_Endpoint_IP vr_endpoint_ip_none()
{
    return (VR_Endpoint_IP) {.kind = VR_Endpoint_IP_Kind_None};
}

VR_Endpoint_IP vr_endpoint_ip_empty(VR_Endpoint_IP_Kind kind)
{
    return (VR_Endpoint_IP) {.kind = kind};
}

VR_Endpoint_IP vr_endpoint_ip_local(VR_Endpoint_IP_Kind kind, uint16 port)
{
    switch (kind) {
        case VR_Endpoint_IP_Kind_V4: {
            VR_Endpoint_IPv4 ipv4 = VR_ENDPOINT_IPV4_LOCAL;

            return (VR_Endpoint_IP) {
                .kind    = kind,
                .ip_ver4 = ipv4,
                .port    = port,
            };
        } break;

        case VR_Endpoint_IP_Kind_V6: {
            VR_Endpoint_IPv6 ipv6 = VR_ENDPOINT_IPV6_LOCAL;

            return (VR_Endpoint_IP) {
                .kind    = kind,
                .ip_ver6 = ipv6,
                .port    = port,
            };
        } break;

        default: break;
    }

    return (VR_Endpoint_IP) {.kind = VR_Endpoint_IP_Kind_None};
}

VR_Endpoint_IP vr_endpoint_ip_ver4(VR_Endpoint_IPv4 ipv4, uint16 port)
{
    return (VR_Endpoint_IP) {
        .kind    = VR_Endpoint_IP_Kind_V4,
        .ip_ver4 = ipv4,
        .port    = port,
    };
}

VR_Endpoint_IP vr_endpoint_ip_ver6(VR_Endpoint_IPv6 ipv6, uint16 port)
{
    return (VR_Endpoint_IP) {
        .kind    = VR_Endpoint_IP_Kind_V6,
        .ip_ver6 = ipv6,
        .port    = port,
    };
}

bool32 vr_endpoint_ip_is_equal(VR_Endpoint_IP self, VR_Endpoint_IP other)
{
    if (self.kind != other.kind || self.port != other.port)
        return 0;

    switch (self.kind) {
        case VR_Endpoint_IP_Kind_V4: {
            for (intptr i = 0; i < VR_ENDPOINT_IPV4_SIZE; i += 1) {
                if (self.ip_ver4.elements[i] != other.ip_ver4.elements[i])
                    return 0;
            }

            return 1;
        } break;

        case VR_Endpoint_IP_Kind_V6: {
            for (intptr i = 0; i < VR_ENDPOINT_IPV6_SIZE; i += 1) {
                if (self.ip_ver6.elements[i] != other.ip_ver6.elements[i])
                    return 0;
            }

            return 1;
        } break;

        default: break;
    }

    return 0;
}

VR_Socket_TCP vr_socket_tcp_reserve(VR_Alloc* alloc)
{
    return (VR_Socket_TCP) {
        .impl = _vr_socket_tcp_reserve_(alloc),
    };
}

bool32 vr_socket_tcp_init(VR_Socket_TCP self, VR_Endpoint_IP_Kind kind)
{
    return _vr_socket_tcp_init_(self.impl, vr_endpoint_ip_empty(kind));
}

bool32 vr_socket_tcp_init_bound(VR_Socket_TCP self, VR_Endpoint_IP_Kind kind, uint16 port)
{
    VR_Endpoint_IP endpoint = vr_endpoint_ip_local(kind, port);
    bool32         status   = 1;

    if (port == 0) return 0;

    status &= _vr_socket_tcp_init_(self.impl, endpoint);
    status &= _vr_socket_tcp_bind_(self.impl);

    if (status == 0)
        _vr_socket_tcp_uninit_(self.impl);

    return status;
}

void vr_socket_tcp_uninit(VR_Socket_TCP self)
{
    return _vr_socket_tcp_uninit_(self.impl);
}

bool32 vr_socket_tcp_listen(VR_Socket_TCP listener)
{
    return _vr_socket_tcp_listen_(listener.impl);
}

bool32 vr_socket_tcp_accept(VR_Socket_TCP self, VR_Socket_TCP listener)
{
    return _vr_socket_tcp_accept_(self.impl, listener.impl);
}

bool32 vr_socket_tcp_connect(VR_Socket_TCP self, VR_Endpoint_IP endpoint)
{
    return _vr_socket_tcp_connect_(self.impl, endpoint);
}

intptr vr_socket_tcp_write(VR_Socket_TCP self, uint8* pntr, intptr size)
{
    return _vr_socket_tcp_write_(self.impl, pntr, size);
}

intptr vr_socket_tcp_write_all(VR_Socket_TCP self, uint8* pntr, intptr size)
{
    intptr result = 0;

    while (result < size) {
        intptr count = _vr_socket_tcp_write_(
            self.impl, pntr, size);

        if (count <= 0 || count > size) break;

        pntr   += count;
        size   -= count;
        result += count;
    }

    return result;
}

intptr vr_socket_tcp_read(VR_Socket_TCP self, uint8* pntr, intptr size)
{
    return _vr_socket_tcp_read_(self.impl, pntr, size);
}

VR_Endpoint_IP vr_socket_tcp_endpoint(VR_Socket_TCP self)
{
    return _vr_socket_tcp_endpoint_(self.impl);
}

VR_Socket_UDP vr_socket_udp_reserve(VR_Alloc* alloc)
{
    return (VR_Socket_UDP) {
        .impl = _vr_socket_udp_reserve_(alloc),
    };
}

bool32 vr_socket_udp_init(VR_Socket_UDP self, VR_Endpoint_IP_Kind kind)
{
    return _vr_socket_udp_init_(self.impl, vr_endpoint_ip_empty(kind));
}

bool32 vr_socket_udp_init_bound(VR_Socket_UDP self, VR_Endpoint_IP_Kind kind, uint16 port)
{
    VR_Endpoint_IP endpoint = vr_endpoint_ip_local(kind, port);
    bool32         status   = 1;

    if (port == 0) return 0;

    status &= _vr_socket_udp_init_(self.impl, endpoint);
    status &= _vr_socket_udp_bind_(self.impl);

    if (status == 0)
        _vr_socket_udp_uninit_(self.impl);

    return status;
}

void vr_socket_udp_uninit(VR_Socket_UDP self)
{
    return _vr_socket_udp_uninit_(self.impl);
}

intptr vr_socket_udp_write(VR_Socket_UDP self, uint8* pntr, intptr size, VR_Endpoint_IP endpoint)
{
    return _vr_socket_udp_write_(self.impl, pntr, size, endpoint);
}

intptr vr_socket_udp_write_all(VR_Socket_UDP self, uint8* pntr, intptr size, VR_Endpoint_IP endpoint)
{
    intptr result = 0;

    while (result < size) {
        intptr count = _vr_socket_udp_write_(
            self.impl, pntr, size, endpoint);

        if (count <= 0 || count > size) break;

        pntr   += count;
        size   -= count;
        result += count;
    }

    return result;
}

intptr vr_socket_udp_read(VR_Socket_UDP self, uint8* pntr, intptr size, VR_Endpoint_IP* endpoint)
{
    return _vr_socket_udp_read_(self.impl, pntr, size, endpoint);
}

VR_Endpoint_IP vr_socket_udp_endpoint(VR_Socket_UDP self)
{
    return _vr_socket_udp_endpoint_(self.impl);
}

#endif
