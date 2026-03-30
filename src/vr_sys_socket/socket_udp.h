#ifndef VR_SYS_SOCKET_SOCKET_UDP_H
#define VR_SYS_SOCKET_SOCKET_UDP_H

#include "endpoint_ip.h"

typedef struct VR_Socket_UDP VR_Socket_UDP;

VR_Socket_UDP* vr_socket_udp_reserve(VR_Alloc alloc);

bool32 vr_socket_udp_create(VR_Socket_UDP* self, VR_Endpoint_IP endpoint);

void vr_socket_udp_destroy(VR_Socket_UDP* self);

bool32 vr_socket_udp_bind(VR_Socket_UDP* self);

intptr vr_socket_udp_write(VR_Socket_UDP* self, uint8* pntr, intptr size, VR_Endpoint_IP endpoint);

intptr vr_socket_udp_read(VR_Socket_UDP* self, uint8* pntr, intptr size, VR_Endpoint_IP* endpoint);

VR_Endpoint_IP vr_socket_udp_get_endpoint(VR_Socket_UDP* self);

#endif
