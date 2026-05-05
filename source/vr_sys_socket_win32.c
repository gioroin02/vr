#ifndef VR_SYS_SOCKET_WIN32_C
#define VR_SYS_SOCKET_WIN32_C

#include "vr_sys_socket_win32.h"

static volatile long vr_win32_socket_refs = 0;

LPFN_CONNECTEX WSAConnectEx = (LPFN_CONNECTEX) NULL;
LPFN_ACCEPTEX  WSAAcceptEx  = (LPFN_ACCEPTEX)  NULL;

static void* _vr_win32_socket_load_(SOCKET handle, GUID guid)
{
    void* result = NULL;
    DWORD bytes  = 0;

    int error = WSAIoctl(handle, SIO_GET_EXTENSION_FUNCTION_POINTER,
        &guid, sizeof guid, &result, sizeof result, &bytes, 0, 0);

    if (error == SOCKET_ERROR) return NULL;

    return result;
}

static bool32 _vr_win32_socket_start_()
{
    WSADATA data = {0};

    if (WSAStartup(MAKEWORD(2, 2), &data) != 0) return 0;

    int flag_handle = WSA_FLAG_OVERLAPPED;
    int flag_ops    = SOCK_STREAM;

    SOCKET handle = WSASocketW(AF_INET, flag_ops, IPPROTO_TCP, 0, 0, flag_handle);

    if (handle == INVALID_SOCKET) return 0;

    WSAConnectEx = _vr_win32_socket_load_(handle, (GUID) WSAID_CONNECTEX);
    WSAAcceptEx  = _vr_win32_socket_load_(handle, (GUID) WSAID_ACCEPTEX);

    if (WSAConnectEx != NULL && WSAAcceptEx != NULL) return 1;

    closesocket(handle);

    vr_win32_socket_stop();

    return 0;
}

static void _vr_win32_socket_stop_()
{
    WSAConnectEx = NULL;
    WSAAcceptEx  = NULL;

    WSACleanup();
}

bool32 vr_win32_socket_start()
{
    while (1) {
        long refs_curr = vr_win32_socket_refs;
        long refs_new  = refs_curr + 1;

        if (refs_curr == VR_INT16_MAX) return 0;

        if (InterlockedCompareExchange(&vr_win32_socket_refs, refs_new, refs_curr) == refs_curr) {
            if (refs_new == 1)
                return _vr_win32_socket_start_();

            return 1;
        }
    }

    return 0;
}

void vr_win32_socket_stop()
{
    while (1) {
        long refs_curr = vr_win32_socket_refs;
        long refs_new  = refs_curr - 1;

        if (refs_curr == VR_INT16_MIN) break;

        if (InterlockedCompareExchange(&vr_win32_socket_refs, refs_new, refs_curr) == refs_curr) {
            if (refs_new == 0)
                _vr_win32_socket_stop_();

            break;
        }
    }
}

sockaddr_storage_t vr_win32_sockaddr_make(VR_Endpoint_IP endpoint)
{
    sockaddr_storage_t result = {0};

    switch (endpoint.kind) {
        case VR_Endpoint_IP_Kind_4: {
            sockaddr_ip4_t* ip_4 = (sockaddr_ip4_t*) &result;

            ip_4->sin_family = AF_INET;
            ip_4->sin_port   = htons(endpoint.port);

            vr_memory_copy(&ip_4->sin_addr.s_addr,
                VR_ENDPOINT_IP4_SIZE, endpoint.ip_4.elems);
        } break;

        case VR_Endpoint_IP_Kind_6: {
            sockaddr_ip6_t* ip_6 = (sockaddr_ip6_t*) &result;

            ip_6->sin6_family = AF_INET6;
            ip_6->sin6_port   = htons(endpoint.port);

            vr_memory_copy(&ip_6->sin6_addr.s6_addr,
                VR_ENDPOINT_IP6_SIZE, endpoint.ip_6.elems);
        } break;

        default: break;
    }

    return result;
}

sockaddr_storage_t vr_win32_sockaddr_make_any(VR_Endpoint_IP_Kind kind, uint16 port)
{
    sockaddr_storage_t result = {0};

    switch (kind) {
        case VR_Endpoint_IP_Kind_4: {
            sockaddr_ip4_t* ip_4        = (sockaddr_ip4_t*) &result;
            uint32          in4addr_any = INADDR_ANY;

            ip_4->sin_family = AF_INET;
            ip_4->sin_port   = htons(port);

            vr_memory_copy(&ip_4->sin_addr.s_addr,
                VR_ENDPOINT_IP4_SIZE, (void*) &in4addr_any);
        } break;

        case VR_Endpoint_IP_Kind_6: {
            sockaddr_ip6_t* ip_6 = (sockaddr_ip6_t*) &result;

            ip_6->sin6_family = AF_INET6;
            ip_6->sin6_port   = htons(port);

            vr_memory_copy(&ip_6->sin6_addr.s6_addr,
                VR_ENDPOINT_IP6_SIZE, (void*) &in6addr_any);
        } break;

        default: break;
    }

    return result;
}

intptr vr_win32_sockaddr_size(sockaddr_storage_t* self)
{
    switch (self->ss_family) {
        case AF_INET:  { return sizeof (sockaddr_ip4_t); } break;
        case AF_INET6: { return sizeof (sockaddr_ip6_t); } break;

        default: break;
    }

    return 0;
}

VR_Endpoint_IP vr_win32_sockaddr_endpoint(sockaddr_storage_t* self)
{
    VR_Endpoint_IP result = vr_endpoint_ip_none();

    switch (self->ss_family) {
        case AF_INET: {
            sockaddr_ip4_t* ip_4 = (sockaddr_ip4_t*) self;

            result.kind = VR_Endpoint_IP_Kind_4;
            result.port = ntohs(ip_4->sin_port);

            vr_memory_copy(result.ip_4.elems,
                VR_ENDPOINT_IP4_SIZE, &ip_4->sin_addr.s_addr);
        } break;

        case AF_INET6: {
            sockaddr_ip6_t* ip_6 = (sockaddr_ip6_t*) self;

            result.kind = VR_Endpoint_IP_Kind_6;
            result.port = ntohs(ip_6->sin6_port);

            vr_memory_copy(result.ip_6.elems,
                VR_ENDPOINT_IP6_SIZE, &ip_6->sin6_addr.s6_addr);
        } break;

        default: break;
    }

    return result;
}

VR_Win32_Socket_TCP* vr_win32_socket_tcp_reserve(VR_Alloc alloc)
{
    return vr_alloc_reserve_of(alloc, 1, VR_Win32_Socket_TCP);
}

bool32 vr_win32_socket_tcp_create(VR_Win32_Socket_TCP* self, VR_Endpoint_IP endpoint)
{
    self->handle  = INVALID_SOCKET;
    self->address = (sockaddr_storage_t) {0};
    self->queue   = NULL;

    sockaddr_storage_t address = vr_win32_sockaddr_make(endpoint);
    socklen_t          length  = vr_win32_sockaddr_size(&address);

    if (length <= 0 || vr_win32_socket_start() == 0) return 0;

    int flag_handle = WSA_FLAG_OVERLAPPED;
    int flag_ops    = SOCK_STREAM;

    SOCKET handle = WSASocketW(address.ss_family, flag_ops,
        IPPROTO_TCP, 0, 0, flag_handle);

    if (handle != INVALID_SOCKET) {
        self->handle  = handle;
        self->address = address;

        return 1;
    }

    vr_win32_socket_stop();

    return 0;
}

void vr_win32_socket_tcp_destroy(VR_Win32_Socket_TCP* self)
{
    if (self->handle == INVALID_SOCKET) return;

    closesocket(self->handle);

    vr_win32_socket_stop();

    self->handle  = INVALID_SOCKET;
    self->address = (sockaddr_storage_t) {0};
    self->queue   = NULL;
}

bool32 vr_win32_socket_tcp_bind(VR_Win32_Socket_TCP* self)
{
    sockaddr_storage_t address = self->address;
    socklen_t          length  = vr_win32_sockaddr_size(&address);

    if (length <= 0) return 0;

    if (bind(self->handle, (sockaddr_t*) &address, length) == SOCKET_ERROR)
        return 0;

    return 1;
}

bool32 vr_win32_socket_tcp_listen(VR_Win32_Socket_TCP* self)
{
    if (listen(self->handle, SOMAXCONN) == SOCKET_ERROR)
        return 0;

    return 1;
}

bool32 vr_win32_socket_tcp_accept(VR_Win32_Socket_TCP* self, VR_Win32_Socket_TCP* listener)
{
    self->handle  = INVALID_SOCKET;
    self->address = (sockaddr_storage_t) {0};

    if (vr_win32_socket_start() == 0) return 0;

    sockaddr_storage_t address = {0};
    socklen_t          length  = sizeof address;

    SOCKET handle = accept(listener->handle, (sockaddr_t*) &address, &length);

    if (handle != INVALID_SOCKET) {
        self->handle  = handle;
        self->address = address;

        return 1;
    }

    vr_win32_socket_stop();

    return 0;
}

bool32 vr_win32_socket_tcp_connect(VR_Win32_Socket_TCP* self, VR_Endpoint_IP endpoint)
{
    sockaddr_storage_t address = vr_win32_sockaddr_make(endpoint);
    socklen_t          length  = vr_win32_sockaddr_size(&address);

    if (length <= 0) return 0;

    if (connect(self->handle, (sockaddr_t*) &address, length) == SOCKET_ERROR)
        return 0;

    return 1;
}

intptr vr_win32_socket_tcp_write(VR_Win32_Socket_TCP* self, uint8* pntr, intptr size)
{
    if (pntr == NULL || size <= 0) return 0;

    intptr result = 0;
    int    count  = 0;

    for (; result < size; result += count) {
        count = send(self->handle, (char*) pntr, (int) size, 0);

        if (count <= 0 || count > size) break;

        pntr += count;
        size -= count;
    }

    return result;
}

intptr vr_win32_socket_tcp_read(VR_Win32_Socket_TCP* self, uint8* pntr, intptr size)
{
    if (pntr == NULL || size <= 0) return 0;

    int count = recv(self->handle, (char*) pntr, (int) size, 0);

    if (count <= 0 || count > size) return 0;

    return (intptr) count;
}

VR_Endpoint_IP vr_win32_socket_tcp_endpoint(VR_Win32_Socket_TCP* self)
{
    return vr_win32_sockaddr_endpoint(&self->address);
}

VR_Win32_Socket_UDP* vr_win32_socket_udp_reserve(VR_Alloc alloc)
{
    return vr_alloc_reserve_of(alloc, 1, VR_Win32_Socket_UDP);
}

bool32 vr_win32_socket_udp_create(VR_Win32_Socket_UDP* self, VR_Endpoint_IP endpoint)
{
    self->handle  = INVALID_SOCKET;
    self->address = (sockaddr_storage_t) {0};
    self->queue   = NULL;

    sockaddr_storage_t address = vr_win32_sockaddr_make(endpoint);
    socklen_t          length  = vr_win32_sockaddr_size(&address);

    if (length <= 0 || vr_win32_socket_start() == 0) return 0;

    int flag_handle = WSA_FLAG_OVERLAPPED;
    int flag_ops    = SOCK_DGRAM;

    SOCKET handle = WSASocketW(address.ss_family, flag_ops,
        IPPROTO_UDP, 0, 0, flag_handle);

    if (handle != INVALID_SOCKET) {
        self->handle  = handle;
        self->address = address;

        return 1;
    }

    vr_win32_socket_stop();

    return 0;
}

void vr_win32_socket_udp_destroy(VR_Win32_Socket_UDP* self)
{
    if (self->handle == INVALID_SOCKET) return;

    closesocket(self->handle);

    vr_win32_socket_stop();

    self->handle  = INVALID_SOCKET;
    self->address = (sockaddr_storage_t) {0};
    self->queue   = NULL;
}

bool32 vr_win32_socket_udp_bind(VR_Win32_Socket_UDP* self)
{
    sockaddr_storage_t address = self->address;
    socklen_t          length  = vr_win32_sockaddr_size(&address);

    if (length <= 0) return 0;

    if (bind(self->handle, (sockaddr_t*) &address, length) == SOCKET_ERROR)
        return 0;

    return 1;
}

intptr vr_win32_socket_udp_write(VR_Win32_Socket_UDP* self, uint8* pntr, intptr size, VR_Endpoint_IP endpoint)
{
    if (pntr == NULL || size <= 0) return 0;

    sockaddr_storage_t address = vr_win32_sockaddr_make(endpoint);
    socklen_t          length  = vr_win32_sockaddr_size(&address);

    intptr result = 0;
    int    count  = 0;

    for (; result < size; result += count) {
        count = sendto(self->handle, (char*) pntr,
            (int) size, 0, (sockaddr_t*) &address, length);

        if (count <= 0 || count > size) break;

        pntr += count;
        size -= count;
    }

    return result;
}

intptr vr_win32_socket_udp_read(VR_Win32_Socket_UDP* self, uint8* pntr, intptr size, VR_Endpoint_IP* endpoint)
{
    if (pntr == NULL || size <= 0) return 0;

    sockaddr_storage_t address = {0};
    socklen_t          length  = sizeof address;

    int count = recvfrom(self->handle, (char*) pntr,
        (int) size, 0, (sockaddr_t*) &address, &length);

    if (count <= 0 || count > size) return 0;

    if (endpoint != NULL)
        *endpoint = vr_win32_sockaddr_endpoint(&address);

    return (intptr) count;
}

VR_Endpoint_IP vr_win32_socket_udp_endpoint(VR_Win32_Socket_UDP* self)
{
    return vr_win32_sockaddr_endpoint(&self->address);
}

#endif
