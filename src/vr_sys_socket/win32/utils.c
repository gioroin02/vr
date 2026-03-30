#ifndef VR_SYS_SOCKET_WIN32_UTILS_C
#define VR_SYS_SOCKET_WIN32_UTILS_C

#include "utils.h"

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

    if (WSAConnectEx != NULL && WSAAcceptEx != NULL)
        return 1;

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

        if (refs_curr == 65535) return 0;

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

        if (refs_curr == 0) break;

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
            ip_4->sin_port   = htons(endpoint.ip_4.port);

            vr_memory_copy(&ip_4->sin_addr.s_addr,
                VR_ENDPOINT_IP4_SIZE, endpoint.ip_4.values);
        } break;

        case VR_Endpoint_IP_Kind_6: {
            sockaddr_ip6_t* ip_6 = (sockaddr_ip6_t*) &result;

            ip_6->sin6_family = AF_INET6;
            ip_6->sin6_port   = htons(endpoint.ip_6.port);

            vr_memory_copy(&ip_6->sin6_addr.s6_addr,
                VR_ENDPOINT_IP6_SIZE, endpoint.ip_6.values);
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

intptr vr_win32_sockaddr_get_size(sockaddr_storage_t* self)
{
    switch (self->ss_family) {
        case AF_INET:  { return sizeof (sockaddr_ip4_t); } break;
        case AF_INET6: { return sizeof (sockaddr_ip6_t); } break;

        default: break;
    }

    return 0;
}

VR_Endpoint_IP vr_win32_sockaddr_get_endpoint(sockaddr_storage_t* self)
{
    VR_Endpoint_IP result = vr_endpoint_ip_none();

    switch (self->ss_family) {
        case AF_INET: {
            sockaddr_ip4_t* ip_4 = (sockaddr_ip4_t*) self;

            result.kind      = VR_Endpoint_IP_Kind_4;
            result.ip_4.port = ntohs(ip_4->sin_port);

            vr_memory_copy(result.ip_4.values,
                VR_ENDPOINT_IP4_SIZE, &ip_4->sin_addr.s_addr);
        } break;

        case AF_INET6: {
            sockaddr_ip6_t* ip_6 = (sockaddr_ip6_t*) self;

            result.kind      = VR_Endpoint_IP_Kind_6;
            result.ip_6.port = ntohs(ip_6->sin6_port);

            vr_memory_copy(result.ip_6.values,
                VR_ENDPOINT_IP6_SIZE, &ip_6->sin6_addr.s6_addr);
        } break;

        default: break;
    }

    return result;
}

#endif
