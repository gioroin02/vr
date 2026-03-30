#ifndef VR_SYS_SOCKET_SOCKET_TCP_H
#define VR_SYS_SOCKET_SOCKET_TCP_H

#include "endpoint_ip.h"

typedef struct VR_Socket_TCP VR_Socket_TCP;

VR_Socket_TCP* vr_socket_tcp_reserve(VR_Alloc alloc);

bool32 vr_socket_tcp_create(VR_Socket_TCP* self, VR_Endpoint_IP endpoint);

void vr_socket_tcp_destroy(VR_Socket_TCP* self);

bool32 vr_socket_tcp_bind(VR_Socket_TCP* self);

bool32 vr_socket_tcp_listen(VR_Socket_TCP* self);

bool32 vr_socket_tcp_accept(VR_Socket_TCP* self, VR_Socket_TCP* listener);

bool32 vr_socket_tcp_connect(VR_Socket_TCP* self, VR_Endpoint_IP endpoint);

intptr vr_socket_tcp_write(VR_Socket_TCP* self, uint8* pntr, intptr size);

intptr vr_socket_tcp_read(VR_Socket_TCP* self, uint8* pntr, intptr size);

VR_Endpoint_IP vr_socket_tcp_get_endpoint(VR_Socket_TCP* self);

#endif
