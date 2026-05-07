#ifndef VR_SYSTEM_SOCKET_H
#define VR_SYSTEM_SOCKET_H

#include "vr_base_memory.h"

typedef enum
{
    VR_Endpoint_IP_Kind_None,
    VR_Endpoint_IP_Kind_V4,
    VR_Endpoint_IP_Kind_V6,
}
VR_Endpoint_IP_Kind;

#define VR_ENDPOINT_IPV4_SIZE ((intptr) 4)

typedef struct
{
    union {
        uint8 elems[VR_ENDPOINT_IPV4_SIZE];

        struct {
            uint8 elem_0, elem_1, elem_2, elem_3;
        };
    };
}
VR_Endpoint_IPv4;

#define VR_ENDPOINT_IPV6_SIZE ((intptr) 16)

typedef struct
{
    union {
        uint8 elems[VR_ENDPOINT_IPV6_SIZE];

        struct {
            uint8  elem_0,  elem_1,  elem_2,  elem_3;
            uint8  elem_4,  elem_5,  elem_6,  elem_7;
            uint8  elem_8,  elem_9, elem_10, elem_11;
            uint8 elem_12, elem_13, elem_14, elem_15;
        };
    };
}
VR_Endpoint_IPv6;

typedef struct
{
    VR_Endpoint_IP_Kind kind;
    uint16              port;

    union {
        VR_Endpoint_IPv4 ip_ver4;
        VR_Endpoint_IPv6 ip_ver6;
    };
}
VR_Endpoint_IP;

VR_Endpoint_IP vr_endpoint_ip_none();

#define vr_endpoint_ipv4_empty() vr_endpoint_ip_empty(VR_Endpoint_IP_Kind_V4)
#define vr_endpoint_ipv6_empty() vr_endpoint_ip_empty(VR_Endpoint_IP_Kind_V6)

VR_Endpoint_IP vr_endpoint_ip_empty(VR_Endpoint_IP_Kind kind);

#define vr_endpoint_ipv4_local(port) vr_endpoint_ip_local(VR_Endpoint_IP_Kind_V4, port)
#define vr_endpoint_ipv6_local(port) vr_endpoint_ip_local(VR_Endpoint_IP_Kind_V6, port)

VR_Endpoint_IP vr_endpoint_ip_local(VR_Endpoint_IP_Kind kind, uint16 port);

bool32 vr_endpoint_ip_is_equal(VR_Endpoint_IP self, VR_Endpoint_IP other);

typedef struct VR_Socket_TCP VR_Socket_TCP;

VR_Socket_TCP* vr_socket_tcp_reserve(VR_Alloc alloc);

bool32 vr_socket_tcp_init(VR_Socket_TCP* self, VR_Endpoint_IP_Kind kind);

bool32 vr_socket_tcp_init_bound(VR_Socket_TCP* self, VR_Endpoint_IP_Kind kind, uint16 port);

bool32 vr_socket_tcp_listen(VR_Socket_TCP* self);

void vr_socket_tcp_deinit(VR_Socket_TCP* self);

bool32 vr_socket_tcp_accept(VR_Socket_TCP* self, VR_Socket_TCP* listener);

bool32 vr_socket_tcp_connect(VR_Socket_TCP* self, VR_Endpoint_IP endpoint);

intptr vr_socket_tcp_write(VR_Socket_TCP* self, uint8* pntr, intptr size);

intptr vr_socket_tcp_read(VR_Socket_TCP* self, uint8* pntr, intptr size);

VR_Endpoint_IP vr_socket_tcp_endpoint(VR_Socket_TCP* self);

typedef struct VR_Socket_UDP VR_Socket_UDP;

VR_Socket_UDP* vr_socket_udp_reserve(VR_Alloc alloc);

bool32 vr_socket_udp_init(VR_Socket_UDP* self, VR_Endpoint_IP_Kind kind);

bool32 vr_socket_udp_init_bound(VR_Socket_UDP* self, VR_Endpoint_IP_Kind kind, uint16 port);

void vr_socket_udp_deinit(VR_Socket_UDP* self);

intptr vr_socket_udp_write(VR_Socket_UDP* self, uint8* pntr, intptr size, VR_Endpoint_IP endpoint);

intptr vr_socket_udp_read(VR_Socket_UDP* self, uint8* pntr, intptr size, VR_Endpoint_IP* endpoint);

VR_Endpoint_IP vr_socket_udp_endpoint(VR_Socket_UDP* self);

#endif
