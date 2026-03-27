#ifndef VR_SYS_SOCKET_ENDPOINT_IP_H
#define VR_SYS_SOCKET_ENDPOINT_IP_H

#include "import.h"

#define VR_ENDPOINT_IP4_SIZE ((intptr) 4)
#define VR_ENDPOINT_IP6_SIZE ((intptr) 16)

#define vr_endpoint_ip4_empty()     vr_endpoint_ip_empty(VR_Endpoint_IP_Kind_4)
#define vr_endpoint_ip4_local(port) vr_endpoint_ip_local(VR_Endpoint_IP_Kind_4, port)
#define vr_endpoint_ip6_empty()     vr_endpoint_ip_empty(VR_Endpoint_IP_Kind_6)
#define vr_endpoint_ip6_local(port) vr_endpoint_ip_local(VR_Endpoint_IP_Kind_6, port)

typedef enum
{
    VR_Endpoint_IP_Kind_None,
    VR_Endpoint_IP_Kind_4,
    VR_Endpoint_IP_Kind_6,
}
VR_Endpoint_IP_Kind;

typedef struct
{
    uint16 port;

    union {
        uint8 values[VR_ENDPOINT_IP4_SIZE];

        struct {
            uint8 value_0, value_1, value_2, value_3;
        };
    };
}
VR_Endpoint_IP4;

typedef struct
{
    uint16 port;

    union {
        uint8 values[VR_ENDPOINT_IP6_SIZE];

        struct {
            uint8  value_0,  value_1,  value_2,  value_3;
            uint8  value_4,  value_5,  value_6,  value_7;
            uint8  value_8,  value_9, value_10, value_11;
            uint8 value_12, value_13, value_14, value_15;
        };
    };
}
VR_Endpoint_IP6;

typedef struct
{
    VR_Endpoint_IP_Kind kind;

    union {
        VR_Endpoint_IP4 ip_4;
        VR_Endpoint_IP6 ip_6;
    };
}
VR_Endpoint_IP;

VR_Endpoint_IP vr_endpoint_ip_none();

VR_Endpoint_IP vr_endpoint_ip_empty(VR_Endpoint_IP_Kind kind);

VR_Endpoint_IP vr_endpoint_ip_local(VR_Endpoint_IP_Kind kind, uint16 port);

bool32 vr_endpoint_ip_is_equal(VR_Endpoint_IP self, VR_Endpoint_IP other);

#endif
