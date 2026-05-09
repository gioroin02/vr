#ifndef VR_LINUX_SOCKET_H
#define VR_LINUX_SOCKET_H

#include "vr_system_socket.h"

#include <unistd.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>

typedef struct sockaddr_storage sockaddr_storage_t;
typedef struct sockaddr         sockaddr_t;
typedef struct sockaddr_in      sockaddr_ip_ver4_t;
typedef struct sockaddr_in6     sockaddr_ip_ver6_t;

sockaddr_storage_t vr_linux_sockaddr_make(VR_Endpoint_IP endpoint);

sockaddr_storage_t vr_linux_sockaddr_make_any(VR_Endpoint_IP_Kind kind, uint16 port);

intptr vr_linux_sockaddr_size(sockaddr_storage_t* self);

VR_Endpoint_IP vr_linux_sockaddr_endpoint(sockaddr_storage_t* self);

typedef struct
{
    int                handle;
    sockaddr_storage_t address;
    void*              queue;
}
VR_Linux_Socket_TCP;

VR_Linux_Socket_TCP* vr_linux_socket_tcp_reserve(VR_Alloc* alloc);

bool32 vr_linux_socket_tcp_init(VR_Linux_Socket_TCP* self, VR_Endpoint_IP endpoint);

void vr_linux_socket_tcp_uninit(VR_Linux_Socket_TCP* self);

bool32 vr_linux_socket_tcp_bind(VR_Linux_Socket_TCP* self);

bool32 vr_linux_socket_tcp_listen(VR_Linux_Socket_TCP* listener);

bool32 vr_linux_socket_tcp_accept(VR_Linux_Socket_TCP* self, VR_Linux_Socket_TCP* listener);

bool32 vr_linux_socket_tcp_connect(VR_Linux_Socket_TCP* self, VR_Endpoint_IP endpoint);

intptr vr_linux_socket_tcp_write(VR_Linux_Socket_TCP* self, uint8* pntr, intptr size);

intptr vr_linux_socket_tcp_read(VR_Linux_Socket_TCP* self, uint8* pntr, intptr size);

VR_Endpoint_IP vr_linux_socket_tcp_endpoint(VR_Linux_Socket_TCP* self);

typedef struct
{
    int                handle;
    sockaddr_storage_t address;
    void*              queue;
}
VR_Linux_Socket_UDP;

VR_Linux_Socket_UDP* vr_linux_socket_udp_reserve(VR_Alloc* alloc);

bool32 vr_linux_socket_udp_init(VR_Linux_Socket_UDP* self, VR_Endpoint_IP endpoint);

void vr_linux_socket_udp_uninit(VR_Linux_Socket_UDP* self);

bool32 vr_linux_socket_udp_bind(VR_Linux_Socket_UDP* self);

intptr vr_linux_socket_udp_write(VR_Linux_Socket_UDP* self, uint8* pntr, intptr size, VR_Endpoint_IP endpoint);

intptr vr_linux_socket_udp_read(VR_Linux_Socket_UDP* self, uint8* pntr, intptr size, VR_Endpoint_IP* endpoint);

VR_Endpoint_IP vr_linux_socket_udp_endpoint(VR_Linux_Socket_UDP* self);

#endif
