#ifndef VR_SYS_SOCKET_H
#define VR_SYS_SOCKET_H

#include "vr_base_memory.h"

typedef enum
{
    VR_Endpoint_IP_Kind_None,
    VR_Endpoint_IP_Kind_4,
    VR_Endpoint_IP_Kind_6,
}
VR_Endpoint_IP_Kind;

#define VR_ENDPOINT_IP4_SIZE ((intptr) 4)

typedef struct
{
    union {
        uint8 elems[VR_ENDPOINT_IP4_SIZE];

        struct {
            uint8 elem_0, elem_1, elem_2, elem_3;
        };
    };
}
VR_Endpoint_IP4;

#define VR_ENDPOINT_IP6_SIZE ((intptr) 16)

typedef struct
{
    union {
        uint8 elems[VR_ENDPOINT_IP6_SIZE];

        struct {
            uint8  elem_0,  elem_1,  elem_2,  elem_3;
            uint8  elem_4,  elem_5,  elem_6,  elem_7;
            uint8  elem_8,  elem_9, elem_10, elem_11;
            uint8 elem_12, elem_13, elem_14, elem_15;
        };
    };
}
VR_Endpoint_IP6;

typedef struct
{
    VR_Endpoint_IP_Kind kind;
    uint16              port;

    union {
        VR_Endpoint_IP4 ip_4;
        VR_Endpoint_IP6 ip_6;
    };
}
VR_Endpoint_IP;

VR_Endpoint_IP vr_endpoint_ip_none();

#define vr_endpoint_ip4_empty()     vr_endpoint_ip_empty(VR_Endpoint_IP_Kind_4)
#define vr_endpoint_ip6_empty()     vr_endpoint_ip_empty(VR_Endpoint_IP_Kind_6)

VR_Endpoint_IP vr_endpoint_ip_empty(VR_Endpoint_IP_Kind kind);

#define vr_endpoint_ip4_local(port) vr_endpoint_ip_local(VR_Endpoint_IP_Kind_4, port)
#define vr_endpoint_ip6_local(port) vr_endpoint_ip_local(VR_Endpoint_IP_Kind_6, port)

VR_Endpoint_IP vr_endpoint_ip_local(VR_Endpoint_IP_Kind kind, uint16 port);

bool32 vr_endpoint_ip_is_equal(VR_Endpoint_IP self, VR_Endpoint_IP other);

typedef struct VR_Socket_TCP VR_Socket_TCP;

VR_Socket_TCP* vr_socket_tcp_reserve(VR_Alloc alloc);

bool32 vr_socket_tcp_create(VR_Socket_TCP* self, VR_Endpoint_IP_Kind kind);

bool32 vr_socket_tcp_create_bound(VR_Socket_TCP* self, VR_Endpoint_IP_Kind kind, uint16 port);

bool32 vr_socket_tcp_listen(VR_Socket_TCP* self);

void vr_socket_tcp_destroy(VR_Socket_TCP* self);

bool32 vr_socket_tcp_accept(VR_Socket_TCP* self, VR_Socket_TCP* listener);

bool32 vr_socket_tcp_connect(VR_Socket_TCP* self, VR_Endpoint_IP endpoint);

intptr vr_socket_tcp_write(VR_Socket_TCP* self, uint8* pntr, intptr size);

intptr vr_socket_tcp_read(VR_Socket_TCP* self, uint8* pntr, intptr size);

VR_Endpoint_IP vr_socket_tcp_endpoint(VR_Socket_TCP* self);

typedef struct VR_Socket_UDP VR_Socket_UDP;

VR_Socket_UDP* vr_socket_udp_reserve(VR_Alloc alloc);

bool32 vr_socket_udp_create(VR_Socket_UDP* self, VR_Endpoint_IP_Kind kind);

bool32 vr_socket_udp_create_bound(VR_Socket_UDP* self, VR_Endpoint_IP_Kind kind, uint16 port);

void vr_socket_udp_destroy(VR_Socket_UDP* self);

intptr vr_socket_udp_write(VR_Socket_UDP* self, uint8* pntr, intptr size, VR_Endpoint_IP endpoint);

intptr vr_socket_udp_read(VR_Socket_UDP* self, uint8* pntr, intptr size, VR_Endpoint_IP* endpoint);

VR_Endpoint_IP vr_socket_udp_endpoint(VR_Socket_UDP* self);

#endif
