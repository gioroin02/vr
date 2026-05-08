#ifndef VR_LINUX_SOCKET_C
#define VR_LINUX_SOCKET_C

#include "vr_linux_socket.h"

sockaddr_storage_t vr_linux_sockaddr_make(VR_Endpoint_IP endpoint)
{
    sockaddr_storage_t result = {0};

    switch (endpoint.kind) {
        case VR_Endpoint_IP_Kind_V4: {
            sockaddr_ipv4_t* ip_ver4 = (sockaddr_ipv4_t*) &result;

            ip_ver4->sin_family = AF_INET;
            ip_ver4->sin_port   = htons(endpoint.port);

            vr_memory_copy(&ip_ver4->sin_addr.s_addr,
                VR_ENDPOINT_IPV4_SIZE, endpoint.ip_ver4.elems);
        } break;

        case VR_Endpoint_IP_Kind_V6: {
            sockaddr_ipv6_t* ip_ver6 = (sockaddr_ipv6_t*) &result;

            ip_ver6->sin6_family = AF_INET6;
            ip_ver6->sin6_port   = htons(endpoint.port);

            vr_memory_copy(&ip_ver6->sin6_addr.s6_addr,
                VR_ENDPOINT_IPV6_SIZE, endpoint.ip_ver6.elems);
        } break;

        default: break;
    }

    return result;
}

sockaddr_storage_t vr_linux_sockaddr_make_any(VR_Endpoint_IP_Kind kind, uint16 port)
{
    sockaddr_storage_t result = {0};

    switch (kind) {
        case VR_Endpoint_IP_Kind_V4: {
            sockaddr_ipv4_t* ip_ver4        = (sockaddr_ipv4_t*) &result;
            uint32          in4addr_any = INADDR_ANY;

            ip_ver4->sin_family = AF_INET;
            ip_ver4->sin_port   = htons(port);

            vr_memory_copy(&ip_ver4->sin_addr.s_addr,
                VR_ENDPOINT_IPV4_SIZE, (void*) &in4addr_any);
        } break;

        case VR_Endpoint_IP_Kind_V6: {
            sockaddr_ipv6_t* ip_ver6 = (sockaddr_ipv6_t*) &result;

            ip_ver6->sin6_family = AF_INET6;
            ip_ver6->sin6_port   = htons(port);

            vr_memory_copy(&ip_ver6->sin6_addr.s6_addr,
                VR_ENDPOINT_IPV6_SIZE, (void*) &in6addr_any);
        } break;

        default: break;
    }

    return result;
}

intptr vr_linux_sockaddr_size(sockaddr_storage_t* self)
{
    switch (self->ss_family) {
        case AF_INET:  { return sizeof (sockaddr_ipv4_t); } break;
        case AF_INET6: { return sizeof (sockaddr_ipv6_t); } break;

        default: break;
    }

    return 0;
}

VR_Endpoint_IP vr_linux_sockaddr_endpoint(sockaddr_storage_t* self)
{
    VR_Endpoint_IP result = vr_endpoint_ip_none();

    switch (self->ss_family) {
        case AF_INET: {
            sockaddr_ipv4_t* ip_ver4 = (sockaddr_ipv4_t*) self;

            result.kind = VR_Endpoint_IP_Kind_V4;
            result.port = ntohs(ip_ver4->sin_port);

            vr_memory_copy(result.ip_ver4.elems,
                VR_ENDPOINT_IPV4_SIZE, &ip_ver4->sin_addr.s_addr);
        } break;

        case AF_INET6: {
            sockaddr_ipv6_t* ip_ver6 = (sockaddr_ipv6_t*) self;

            result.kind = VR_Endpoint_IP_Kind_V6;
            result.port = ntohs(ip_ver6->sin6_port);

            vr_memory_copy(result.ip_ver6.elems,
                VR_ENDPOINT_IPV6_SIZE, &ip_ver6->sin6_addr.s6_addr);
        } break;

        default: break;
    }

    return result;
}

VR_Linux_Socket_TCP* vr_linux_socket_tcp_reserve(VR_Alloc alloc)
{
    return vr_alloc_reserve_of(alloc, 1, VR_Linux_Socket_TCP);
}

bool32 vr_linux_socket_tcp_init(VR_Linux_Socket_TCP* self, VR_Endpoint_IP endpoint)
{
    self->handle  = -1;
    self->address = (sockaddr_storage_t) {0};
    self->queue   = NULL;

    sockaddr_storage_t address = vr_linux_sockaddr_make(endpoint);
    socklen_t          length  = vr_linux_sockaddr_size(&address);

    if (length <= 0) return 0;

    int flag_ops = SOCK_STREAM | SOCK_CLOEXEC;
    int option   = 1;
    int status   = 0;

    int handle = socket(address.ss_family, flag_ops, IPPROTO_TCP);

    if (handle == -1) return 0;

    do {
        status = setsockopt(handle, SOL_SOCKET, SO_REUSEADDR,
            &option, sizeof option);
    }
    while (status == -1 && errno == EINTR);

    if (status != -1) {
        self->handle  = handle;
        self->address = address;

        return 1;
    }

    do {
        status = close(handle);
    }
    while (status == -1 && errno == EINTR);

    return 0;
}

void vr_linux_socket_tcp_uninit(VR_Linux_Socket_TCP* self)
{
    if (self->handle != -1) {
        int status = 0;

        do {
            status = close(self->handle);
        }
        while (status == -1 && errno == EINTR);
    }

    self->handle  = -1;
    self->address = (sockaddr_storage_t) {0};
    self->queue   = NULL;
}

bool32 vr_linux_socket_tcp_bind(VR_Linux_Socket_TCP* self)
{
    sockaddr_storage_t address = self->address;
    socklen_t          length  = vr_linux_sockaddr_size(&address);
    int                status  = 0;

    if (length <= 0) return 0;

    do {
        status = bind(self->handle, (sockaddr_t*) &address, length);
    }
    while (status == -1 && errno == EINTR);

    return status != -1 ? 1 : 0;
}

bool32 vr_linux_socket_tcp_listen(VR_Linux_Socket_TCP* self)
{
    int status = 0;

    do {
        status = listen(self->handle, SOMAXCONN);
    }
    while (status == -1 && errno == EINTR);

    return status != -1 ? 1 : 0;
}

bool32 vr_linux_socket_tcp_accept(VR_Linux_Socket_TCP* self, VR_Linux_Socket_TCP* listener)
{
    self->handle  = -1;
    self->address = (sockaddr_storage_t) {0};

    sockaddr_storage_t address = {0};
    socklen_t          length  = sizeof address;

    int handle = 0;

    do {
        handle = accept(listener->handle, (sockaddr_t*) &address, &length);
    }
    while (handle == -1 && errno == EINTR);

    if (handle != -1) {
        self->handle  = handle;
        self->address = address;

        return 1;
    }

    return 0;
}

bool32 vr_linux_socket_tcp_connect(VR_Linux_Socket_TCP* self, VR_Endpoint_IP endpoint)
{
    sockaddr_storage_t address = vr_linux_sockaddr_make(endpoint);
    socklen_t          length  = vr_linux_sockaddr_size(&address);
    int                status  = 0;

    if (length <= 0) return 0;

    do {
        status = connect(self->handle, (sockaddr_t*) &address, length);
    }
    while (status == -1 && errno == EINTR);

    return status != -1 ? 1 : 0;
}

intptr vr_linux_socket_tcp_write(VR_Linux_Socket_TCP* self, uint8* pntr, intptr size)
{
    if (pntr == NULL || size <= 0) return 0;

    intptr result = 0;
    int    count  = 0;

    for (; result < size; result += count) {
        do {
            count = send(self->handle, (char*) pntr, (int) size, 0);
        }
        while (count == -1 && errno == EINTR);

        if (count <= 0 || count > size) break;

        pntr += count;
        size -= count;
    }

    return result;
}

intptr vr_linux_socket_tcp_read(VR_Linux_Socket_TCP* self, uint8* pntr, intptr size)
{
    if (pntr == NULL || size <= 0) return 0;

    int count = 0;

    do {
        count = recv(self->handle, (char*) pntr, (int) size, 0);
    }
    while (count == -1 && errno == EINTR);

    if (count <= 0 || count > size) return 0;

    return (intptr) count;
}

VR_Endpoint_IP vr_linux_socket_tcp_endpoint(VR_Linux_Socket_TCP* self)
{
    return vr_linux_sockaddr_endpoint(&self->address);
}

VR_Linux_Socket_UDP* vr_linux_socket_udp_reserve(VR_Alloc alloc)
{
    return vr_alloc_reserve_of(alloc, 1, VR_Linux_Socket_UDP);
}

bool32 vr_linux_socket_udp_init(VR_Linux_Socket_UDP* self, VR_Endpoint_IP endpoint)
{
    self->handle  = -1;
    self->address = (sockaddr_storage_t) {0};
    self->queue   = NULL;

    sockaddr_storage_t address = vr_linux_sockaddr_make(endpoint);
    socklen_t          length  = vr_linux_sockaddr_size(&address);

    if (length <= 0) return 0;

    int flag_ops = SOCK_DGRAM | SOCK_CLOEXEC;
    int option   = 1;
    int status   = 0;

    int handle = socket(address.ss_family, flag_ops, IPPROTO_UDP);

    if (handle == -1) return 0;

    do {
        status = setsockopt(handle, SOL_SOCKET, SO_REUSEADDR,
            &option, sizeof option);
    }
    while (status == -1 && errno == EINTR);

    if (status != -1) {
        self->handle  = handle;
        self->address = address;

        return 1;
    }

    do {
        status = close(handle);
    }
    while (status == -1 && errno == EINTR);

    return 0;
}

void vr_linux_socket_udp_uninit(VR_Linux_Socket_UDP* self)
{
    if (self->handle != -1 ) {
        int status = 0;

        do {
            status = close(self->handle);
        }
        while (status == -1 && errno == EINTR);
    }

    self->handle  = -1;
    self->address = (sockaddr_storage_t) {0};
    self->queue   = NULL;
}

bool32 vr_linux_socket_udp_bind(VR_Linux_Socket_UDP* self)
{
    sockaddr_storage_t address = self->address;
    socklen_t          length  = vr_linux_sockaddr_size(&address);
    int                status  = 0;

    if (length <= 0) return 0;

    do {
        status = bind(self->handle, (sockaddr_t*) &address, length);
    }
    while (status == -1 && errno == EINTR);

    return status != -1 ? 1 : 0;
}

intptr vr_linux_socket_udp_write(VR_Linux_Socket_UDP* self, uint8* pntr, intptr size, VR_Endpoint_IP endpoint)
{
    if (pntr == NULL || size <= 0) return 0;

    sockaddr_storage_t address = vr_linux_sockaddr_make(endpoint);
    socklen_t          length  = vr_linux_sockaddr_size(&address);

    intptr result = 0;
    int    count  = 0;

    for (; result < size; result += count) {
        do {
            count = sendto(self->handle, (char*) pntr, (int) size, 0,
                (sockaddr_t*) &address, length);
        }
        while (count == -1 && errno == EINTR);

        if (count <= 0 || count > size) break;

        pntr += count;
        size -= count;
    }

    return result;
}

intptr vr_linux_socket_udp_read(VR_Linux_Socket_UDP* self, uint8* pntr, intptr size, VR_Endpoint_IP* endpoint)
{
    if (pntr == NULL || size <= 0) return 0;

    sockaddr_storage_t address = {0};
    socklen_t          length  = sizeof address;

    int count = 0;

    do {
        count = recvfrom(self->handle, (char*) pntr, (int) size, 0,
            (sockaddr_t*) &address, &length);
    }
    while (count == -1 && errno == EINTR);

    if (count <= 0 || count > size) return 0;

    if (endpoint != NULL)
        *endpoint = vr_linux_sockaddr_endpoint(&address);

    return (intptr) count;
}

VR_Endpoint_IP vr_linux_socket_udp_endpoint(VR_Linux_Socket_UDP* self)
{
    return vr_linux_sockaddr_endpoint(&self->address);
}

#endif
