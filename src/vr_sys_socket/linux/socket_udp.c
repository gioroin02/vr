#ifndef VR_SYS_SOCKET_LINUX_SOCKET_UDP_C
#define VR_SYS_SOCKET_LINUX_SOCKET_UDP_C

#include "socket_udp.h"

VR_Linux_Socket_UDP* vr_linux_socket_udp_reserve(VR_Alloc alloc)
{
    return vr_alloc_reserve_of(alloc, 1, VR_Linux_Socket_UDP);
}

bool32 vr_linux_socket_udp_create(VR_Linux_Socket_UDP* self, VR_Endpoint_IP endpoint)
{
    self->handle = -1;
    self->address = (sockaddr_storage_t) {0};

    sockaddr_storage_t address = vr_linux_sockaddr_make(endpoint);
    socklen_t          length  = vr_linux_sockaddr_get_size(&address);

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

void vr_linux_socket_udp_destroy(VR_Linux_Socket_UDP* self)
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
}

rbool32 vr_linux_socket_udp_bind(VR_Linux_Socket_UDP* self)
{
    sockaddr_storage_t address = self->address;
    socklen_t          length  = vr_linux_sockaddr_get_size(&address);
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
    socklen_t          length  = vr_linux_sockaddr_get_size(&address);

    intptr result = 0;
    int    count  = 0;

    for (NULL; result < size; result += count) {
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
        *endpoint = vr_linux_sockaddr_get_endpoint(&address);

    return (intptr) count;
}

VR_Endpoint_IP vr_linux_socket_udp_get_endpoint(VR_Linux_Socket_UDP* self)
{
    return vr_linux_sockaddr_get_endpoint(&self->address);
}

#endif
