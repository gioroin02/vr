#ifndef VR_SYS_SOCKET_ENDPOINT_IP_C
#define VR_SYS_SOCKET_ENDPOINT_IP_C

#include "endpoint_ip.h"

#define VR_ENDPOINT_IP4_EMPTY ((VR_Endpoint_IP4) {0})
#define VR_ENDPOINT_IP4_LOCAL ((VR_Endpoint_IP4) {.value_0 = 0x7F, .value_3 = 0x01})
#define VR_ENDPOINT_IP6_EMPTY ((VR_Endpoint_IP6) {0})
#define VR_ENDPOINT_IP6_LOCAL ((VR_Endpoint_IP6) {.value_15 = 0x01})

VR_Endpoint_IP vr_endpoint_ip_none()
{
    return (VR_Endpoint_IP) {.kind = VR_Endpoint_IP_Kind_None};
}

VR_Endpoint_IP vr_endpoint_ip_empty(VR_Endpoint_IP_Kind kind)
{
    VR_Endpoint_IP result = vr_endpoint_ip_none();

    switch (kind) {
        case VR_Endpoint_IP_Kind_4: {
            result = (VR_Endpoint_IP) {
                .kind = kind,
                .ip_4 = VR_ENDPOINT_IP4_EMPTY,
            };
        } break;

        case VR_Endpoint_IP_Kind_6: {
            result = (VR_Endpoint_IP) {
                .kind = kind,
                .ip_6 = VR_ENDPOINT_IP6_EMPTY,
            };
        } break;

        default: break;
    }

    return result;
}

VR_Endpoint_IP vr_endpoint_ip_local(VR_Endpoint_IP_Kind kind, uint16 port)
{
    VR_Endpoint_IP result = vr_endpoint_ip_none();

    switch (kind) {
        case VR_Endpoint_IP_Kind_4: {
            result = (VR_Endpoint_IP) {
                .kind = kind,
                .ip_4 = VR_ENDPOINT_IP4_LOCAL,
            };

            result.ip_4.port = port;
        } break;

        case VR_Endpoint_IP_Kind_6: {
            result = (VR_Endpoint_IP) {
                .kind = kind,
                .ip_6 = VR_ENDPOINT_IP6_LOCAL,
            };

            result.ip_6.port = port;
        } break;

        default: break;
    }

    return result;
}

bool32 vr_endpoint_ip_is_equal(VR_Endpoint_IP self, VR_Endpoint_IP other)
{
    if (self.kind != other.kind) return 0;

    switch (self.kind) {
        case VR_Endpoint_IP_Kind_4: {
            if (self.ip_4.port != other.ip_4.port) return 0;

            for (intptr i = 0; i < VR_ENDPOINT_IP4_SIZE; i += 1) {
                if (self.ip_4.values[i] != other.ip_4.values[i])
                    return 0;
            }

            return 1;
        } break;

        case VR_Endpoint_IP_Kind_6: {
            if (self.ip_6.port != other.ip_6.port) return 0;

            for (intptr i = 0; i < VR_ENDPOINT_IP6_SIZE; i += 1) {
                if (self.ip_6.values[i] != other.ip_6.values[i])
                    return 0;
            }

            return 1;
        } break;

        default: break;
    }

    return 0;
}

#endif
