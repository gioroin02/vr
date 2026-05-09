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

#define VR_ENDPOINT_IPV4_SIZE  ((intptr) 4)
#define VR_ENDPOINT_IPV4_EMPTY ((VR_Endpoint_IPv4) {0})
#define VR_ENDPOINT_IPV4_LOCAL ((VR_Endpoint_IPv4) {.elem_0 = 0x7F, .elem_3 = 0x01})

typedef union
{
    uint8 elements[VR_ENDPOINT_IPV4_SIZE];

    struct {
        uint8 elem_0, elem_1, elem_2, elem_3;
    };
}
VR_Endpoint_IPv4;

#define VR_ENDPOINT_IPV6_SIZE  ((intptr) 16)
#define VR_ENDPOINT_IPV6_EMPTY ((VR_Endpoint_IPv6) {0})
#define VR_ENDPOINT_IPV6_LOCAL ((VR_Endpoint_IPv6) {.elem_15 = 0x01})

typedef union
{
    uint8 elements[VR_ENDPOINT_IPV6_SIZE];

    struct {
        uint8 elem_0,  elem_1,  elem_2,  elem_3;
        uint8 elem_4,  elem_5,  elem_6,  elem_7;
        uint8 elem_8,  elem_9,  elem_10, elem_11;
        uint8 elem_12, elem_13, elem_14, elem_15;
    };
}
VR_Endpoint_IPv6;

typedef struct
{
    VR_Endpoint_IP_Kind kind;

    union {
        VR_Endpoint_IPv4 ip_ver4;
        VR_Endpoint_IPv6 ip_ver6;
    };

    uint16 port;
}
VR_Endpoint_IP;

VR_Endpoint_IP vr_endpoint_ip_none();

VR_Endpoint_IP vr_endpoint_ip_empty(VR_Endpoint_IP_Kind kind);

VR_Endpoint_IP vr_endpoint_ip_local(VR_Endpoint_IP_Kind kind, uint16 port);

VR_Endpoint_IP vr_endpoint_ip_ver4(VR_Endpoint_IPv4 ipv4, uint16 port);

VR_Endpoint_IP vr_endpoint_ip_ver6(VR_Endpoint_IPv6 ipv6, uint16 port);

bool32 vr_endpoint_ip_is_equal(VR_Endpoint_IP self, VR_Endpoint_IP other);

typedef struct
{
    void* impl;
}
VR_Socket_TCP;

VR_Socket_TCP vr_socket_tcp_reserve(VR_Alloc* alloc);

bool32 vr_socket_tcp_init(VR_Socket_TCP self, VR_Endpoint_IP_Kind kind);

bool32 vr_socket_tcp_init_bound(VR_Socket_TCP self, VR_Endpoint_IP_Kind kind, uint16 port);

bool32 vr_socket_tcp_listen(VR_Socket_TCP listener);

void vr_socket_tcp_uninit(VR_Socket_TCP self);

bool32 vr_socket_tcp_accept(VR_Socket_TCP self, VR_Socket_TCP listener);

bool32 vr_socket_tcp_connect(VR_Socket_TCP self, VR_Endpoint_IP endpoint);

intptr vr_socket_tcp_write(VR_Socket_TCP self, uint8* pntr, intptr size);

intptr vr_socket_tcp_read(VR_Socket_TCP self, uint8* pntr, intptr size);

VR_Endpoint_IP vr_socket_tcp_endpoint(VR_Socket_TCP self);

typedef struct
{
    void* impl;
}
VR_Socket_UDP;

VR_Socket_UDP vr_socket_udp_reserve(VR_Alloc* alloc);

bool32 vr_socket_udp_init(VR_Socket_UDP self, VR_Endpoint_IP_Kind kind);

bool32 vr_socket_udp_init_bound(VR_Socket_UDP self, VR_Endpoint_IP_Kind kind, uint16 port);

void vr_socket_udp_uninit(VR_Socket_UDP self);

intptr vr_socket_udp_write(VR_Socket_UDP self, uint8* pntr, intptr size, VR_Endpoint_IP endpoint);

intptr vr_socket_udp_read(VR_Socket_UDP self, uint8* pntr, intptr size, VR_Endpoint_IP* endpoint);

VR_Endpoint_IP vr_socket_udp_endpoint(VR_Socket_UDP self);

#endif
