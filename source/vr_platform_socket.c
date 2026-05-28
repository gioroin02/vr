#ifndef VR_PLATFORM_SOCKET_C
#define VR_PLATFORM_SOCKET_C

#define VR_PLATFORM_SOCKET_PRIVATE
#include "vr_platform_socket.h"

#if VR_SYSTEM == VR_SYSTEM_WINDOWS

    LPFN_CONNECTEX WSAConnectEx = (LPFN_CONNECTEX) NULL;
    LPFN_ACCEPTEX  WSAAcceptEx  = (LPFN_ACCEPTEX)  NULL;

    static volatile long vr_win32_socket_refs = 0;

    void* _vr_win32_socket_load_(SOCKET handle, GUID guid)
    {
        void* result = NULL;
        DWORD bytes  = 0;

        int error = WSAIoctl(handle, SIO_GET_EXTENSION_FUNCTION_POINTER,
            &guid, sizeof guid, &result, sizeof result, &bytes, 0, 0);

        if (error == SOCKET_ERROR) return NULL;

        return result;
    }

    VrBool32 _vr_win32_socket_start_(void)
    {
        WSADATA data = {0};

        if (WSAStartup(MAKEWORD(2, 2), &data) != 0) return 0;

        int flag_handle = WSA_FLAG_OVERLAPPED;
        int flag_ops    = SOCK_STREAM;

        SOCKET handle = WSASocketW(AF_INET, flag_ops, IPPROTO_TCP, 0, 0, flag_handle);

        if (handle == INVALID_SOCKET) return 0;

        *((void**) &WSAConnectEx) = _vr_win32_socket_load_(handle, (GUID) WSAID_CONNECTEX);
        *((void**) &WSAAcceptEx)  = _vr_win32_socket_load_(handle, (GUID) WSAID_ACCEPTEX);

        if (WSAConnectEx != NULL && WSAAcceptEx != NULL) return 1;

        closesocket(handle);

        vr_win32_socket_stop();

        return 0;
    }

    void _vr_win32_socket_stop_(void)
    {
        WSAConnectEx = NULL;
        WSAAcceptEx  = NULL;

        WSACleanup();
    }

    VrBool32 vr_win32_socket_start(void)
    {
        while (1) {
            int refs_curr = vr_win32_socket_refs;
            int refs_new  = refs_curr + 1;

            if (refs_curr == VR_UINT16_MAX) return 0;

            if (InterlockedCompareExchange(&vr_win32_socket_refs, refs_new, refs_curr) == refs_curr) {
                if (refs_new == 1)
                    return _vr_win32_socket_start_();

                return 1;
            }
        }

        return 0;
    }

    void vr_win32_socket_stop(void)
    {
        while (1) {
            int refs_curr = vr_win32_socket_refs;
            int refs_new  = refs_curr - 1;

            if (refs_curr == VR_UINT16_MIN) break;

            if (InterlockedCompareExchange(&vr_win32_socket_refs, refs_new, refs_curr) == refs_curr) {
                if (refs_new == 0)
                    _vr_win32_socket_stop_();

                break;
            }
        }
    }

    sockaddr_storage_t vr_win32_sockaddr_make(VrAddressIp addr)
    {
        sockaddr_storage_t result = {0};

        switch (addr.kind) {
            case VrAddressIpKind_V4: {
                sockaddr_ipv4_t* ipv4 = (sockaddr_ipv4_t*) &result;

                ipv4->sin_family = AF_INET;
                ipv4->sin_port   = htons(addr.port);

                vr_memory_copy(&ipv4->sin_addr.s_addr,
                    VR_ADDRESS_IP_VER4_SIZE, addr.ip.v4.elements.array);
            } break;

            case VrAddressIpKind_V6: {
                sockaddr_ipv6_t* ipv6 = (sockaddr_ipv6_t*) &result;

                ipv6->sin6_family = AF_INET6;
                ipv6->sin6_port   = htons(addr.port);

                vr_memory_copy(&ipv6->sin6_addr.s6_addr,
                    VR_ADDRESS_IP_VER6_SIZE, addr.ip.v6.elements.array);
            } break;

            default: break;
        }

        return result;
    }

    sockaddr_storage_t vr_win32_sockaddr_make_any(VrAddressIpKind kind, VrUint16 port)
    {
        sockaddr_storage_t result = {0};

        VrUint32 in4addr_any = INADDR_ANY;

        switch (kind) {
            case VrAddressIpKind_V4: {
                sockaddr_ipv4_t* ipv4 = (sockaddr_ipv4_t*) &result;

                ipv4->sin_family = AF_INET;
                ipv4->sin_port   = htons(port);

                vr_memory_copy(&ipv4->sin_addr.s_addr,
                    VR_ADDRESS_IP_VER4_SIZE, (void*) &in4addr_any);
            } break;

            case VrAddressIpKind_V6: {
                sockaddr_ipv6_t* ipv6 = (sockaddr_ipv6_t*) &result;

                ipv6->sin6_family = AF_INET6;
                ipv6->sin6_port   = htons(port);

                vr_memory_copy(&ipv6->sin6_addr.s6_addr,
                    VR_ADDRESS_IP_VER6_SIZE, (void*) &in6addr_any);
            } break;

            default: break;
        }

        return result;
    }

    VrSint vr_win32_sockaddr_size(sockaddr_storage_t* self)
    {
        switch (self->ss_family) {
            case AF_INET:  { return sizeof (sockaddr_ipv4_t); } break;
            case AF_INET6: { return sizeof (sockaddr_ipv6_t); } break;

            default: break;
        }

        return 0;
    }

    VrAddressIp vr_win32_sockaddr_addr(sockaddr_storage_t* self)
    {
        VrAddressIp result = vr_address_ip_none();

        switch (self->ss_family) {
            case AF_INET: {
                sockaddr_ipv4_t* ipv4 = (sockaddr_ipv4_t*) self;

                result.kind = VrAddressIpKind_V4;
                result.port = ntohs(ipv4->sin_port);

                vr_memory_copy(result.ip.v4.elements.array,
                    VR_ADDRESS_IP_VER4_SIZE, &ipv4->sin_addr.s_addr);
            } break;

            case AF_INET6: {
                sockaddr_ipv6_t* ipv6 = (sockaddr_ipv6_t*) self;

                result.kind = VrAddressIpKind_V6;
                result.port = ntohs(ipv6->sin6_port);

                vr_memory_copy(result.ip.v6.elements.array,
                    VR_ADDRESS_IP_VER6_SIZE, &ipv6->sin6_addr.s6_addr);
            } break;

            default: break;
        }

        return result;
    }

    VrWin32TcpListener* vr_win32_tcp_listener_reserve(VrAlloc* alloc)
    {
        return vr_alloc_reserve_of(alloc, 1, VrWin32TcpListener);
    }

    VrBool32 vr_win32_tcp_listener_init(VrWin32TcpListener* self, VrAddressIp addr)
    {
        self->handle  = INVALID_SOCKET;
        self->queue   = NULL;
        self->address = (sockaddr_storage_t) {0};

        sockaddr_storage_t address = vr_win32_sockaddr_make(addr);
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

    void vr_win32_tcp_listener_uninit(VrWin32TcpListener* self)
    {
        if (self->handle == INVALID_SOCKET) return;

        closesocket(self->handle);

        vr_win32_socket_stop();

        self->handle  = INVALID_SOCKET;
        self->queue   = NULL;
        self->address = (sockaddr_storage_t) {0};
    }

    VrBool32 vr_win32_tcp_listener_bind(VrWin32TcpListener* self)
    {
        socklen_t length = vr_win32_sockaddr_size(&self->address);

        if (length <= 0) return 0;

        int status = bind(self->handle,
            (sockaddr_t*) &self->address, length);

        return status != SOCKET_ERROR ? 1 : 0;
    }

    VrBool32 vr_win32_tcp_listener_listen(VrWin32TcpListener* listener)
    {
        if (listen(listener->handle, SOMAXCONN) == SOCKET_ERROR)
            return 0;

        return 1;
    }

    VrAddressIp vr_win32_tcp_listener_addr(VrWin32TcpListener* self)
    {
        return vr_win32_sockaddr_addr(&self->address);
    }

    #define _vr_tcp_listener_reserve_ vr_win32_tcp_listener_reserve
    #define _vr_tcp_listener_init_    vr_win32_tcp_listener_init
    #define _vr_tcp_listener_uninit_  vr_win32_tcp_listener_uninit
    #define _vr_tcp_listener_bind_    vr_win32_tcp_listener_bind
    #define _vr_tcp_listener_listen_  vr_win32_tcp_listener_listen
    #define _vr_tcp_listener_addr_    vr_win32_tcp_listener_addr

    VrWin32TcpSocket* vr_win32_tcp_socket_reserve(VrAlloc* alloc)
    {
        return vr_alloc_reserve_of(alloc, 1, VrWin32TcpSocket);
    }

    VrBool32 vr_win32_tcp_socket_init(VrWin32TcpSocket* self, VrAddressIp addr)
    {
        self->handle  = INVALID_SOCKET;
        self->queue   = NULL;
        self->address = (sockaddr_storage_t) {0};

        sockaddr_storage_t address = vr_win32_sockaddr_make(addr);
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

    void vr_win32_tcp_socket_uninit(VrWin32TcpSocket* self)
    {
        if (self->handle == INVALID_SOCKET) return;

        closesocket(self->handle);

        vr_win32_socket_stop();

        self->handle  = INVALID_SOCKET;
        self->queue   = NULL;
        self->address = (sockaddr_storage_t) {0};
    }

    VrBool32 vr_win32_tcp_socket_bind(VrWin32TcpSocket* self)
    {
        socklen_t length = vr_win32_sockaddr_size(&self->address);

        if (length <= 0) return 0;

        int status = bind(self->handle,
            (sockaddr_t*) &self->address, length);

        return status != SOCKET_ERROR ? 1 : 0;
    }

    VrBool32 vr_win32_tcp_socket_accept(VrWin32TcpSocket* self, VrWin32TcpListener* listener)
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

    VrBool32 vr_win32_tcp_socket_connect(VrWin32TcpSocket* self, VrAddressIp addr)
    {
        sockaddr_storage_t address = vr_win32_sockaddr_make(addr);
        socklen_t          length  = vr_win32_sockaddr_size(&address);

        if (length <= 0) return 0;

        if (connect(self->handle, (sockaddr_t*) &address, length) == SOCKET_ERROR)
            return 0;

        return 1;
    }

    VrSint vr_win32_tcp_socket_write(VrWin32TcpSocket* self, VrUint8* pntr, VrSint size)
    {
        if (pntr == NULL || size <= 0) return 0;

        int count = send(self->handle, (VrChar8*) pntr, (VrSint32) size, 0);

        if (count <= 0 || count > size) return 0;

        return count;
    }

    VrSint vr_win32_tcp_socket_read(VrWin32TcpSocket* self, VrUint8* pntr, VrSint size)
    {
        if (pntr == NULL || size <= 0) return 0;

        int count = recv(self->handle, (VrChar8*) pntr, (VrSint32) size, 0);

        if (count <= 0 || count > size) return 0;

        return count;
    }

    VrAddressIp vr_win32_tcp_socket_addr(VrWin32TcpSocket* self)
    {
        return vr_win32_sockaddr_addr(&self->address);
    }

    #define _vr_tcp_socket_reserve_ vr_win32_tcp_socket_reserve
    #define _vr_tcp_socket_init_    vr_win32_tcp_socket_init
    #define _vr_tcp_socket_uninit_  vr_win32_tcp_socket_uninit
    #define _vr_tcp_socket_bind_    vr_win32_tcp_socket_bind
    #define _vr_tcp_socket_accept_  vr_win32_tcp_socket_accept
    #define _vr_tcp_socket_connect_ vr_win32_tcp_socket_connect
    #define _vr_tcp_socket_write_   vr_win32_tcp_socket_write
    #define _vr_tcp_socket_read_    vr_win32_tcp_socket_read
    #define _vr_tcp_socket_addr_    vr_win32_tcp_socket_addr

    VrWin32UdpSocket* vr_win32_udp_socket_reserve(VrAlloc* alloc)
    {
        return vr_alloc_reserve_of(alloc, 1, VrWin32UdpSocket);
    }

    VrBool32 vr_win32_udp_socket_init(VrWin32UdpSocket* self, VrAddressIp addr)
    {
        self->handle  = INVALID_SOCKET;
        self->queue   = NULL;
        self->address = (sockaddr_storage_t) {0};

        sockaddr_storage_t address = vr_win32_sockaddr_make(addr);
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

    void vr_win32_udp_socket_uninit(VrWin32UdpSocket* self)
    {
        if (self->handle == INVALID_SOCKET) return;

        closesocket(self->handle);

        vr_win32_socket_stop();

        self->handle  = INVALID_SOCKET;
        self->queue   = NULL;
        self->address = (sockaddr_storage_t) {0};
    }

    VrBool32 vr_win32_udp_socket_bind(VrWin32UdpSocket* self)
    {
        socklen_t length = vr_win32_sockaddr_size(&self->address);

        if (length <= 0) return 0;

        int status = bind(self->handle,
            (sockaddr_t*) &self->address, length);

        return status != SOCKET_ERROR ? 1 : 0;
    }

    VrSint vr_win32_udp_socket_write(VrWin32UdpSocket* self, VrUint8* pntr, VrSint size, VrAddressIp addr)
    {
        if (pntr == NULL || size <= 0) return 0;

        sockaddr_storage_t address = vr_win32_sockaddr_make(addr);
        socklen_t          length  = vr_win32_sockaddr_size(&address);

        int count = sendto(self->handle, (VrChar8*) pntr,
            (VrSint32) size, 0, (sockaddr_t*) &address, length);

        if (count <= 0 || count > size) return 0;

        return count;
    }

    VrSint vr_win32_udp_socket_read(VrWin32UdpSocket* self, VrUint8* pntr, VrSint size, VrAddressIp* addr)
    {
        if (pntr == NULL || size <= 0) return 0;

        sockaddr_storage_t address = {0};
        socklen_t          length  = sizeof address;

        int count = recvfrom(self->handle, (VrChar8*) pntr,
            (VrSint32) size, 0, (sockaddr_t*) &address, &length);

        if (count <= 0 || count > size) return 0;

        if (addr != NULL)
            *addr = vr_win32_sockaddr_addr(&address);

        return count;
    }

    VrAddressIp vr_win32_udp_socket_addr(VrWin32UdpSocket* self)
    {
        return vr_win32_sockaddr_addr(&self->address);
    }

    #define _vr_udp_socket_reserve_ vr_win32_udp_socket_reserve
    #define _vr_udp_socket_init_    vr_win32_udp_socket_init
    #define _vr_udp_socket_uninit_  vr_win32_udp_socket_uninit
    #define _vr_udp_socket_bind_    vr_win32_udp_socket_bind
    #define _vr_udp_socket_write_   vr_win32_udp_socket_write
    #define _vr_udp_socket_read_    vr_win32_udp_socket_read
    #define _vr_udp_socket_addr_    vr_win32_tcp_socket_addr

#elif VR_SYSTEM == VR_SYSTEM_LINUX

    sockaddr_storage_t vr_linux_sockaddr_make(VrAddressIp addr)
    {
        sockaddr_storage_t result = {0};

        switch (addr.kind) {
            case VrAddressIpKind_V4: {
                sockaddr_ipv4_t* ipv4 = (sockaddr_ipv4_t*) &result;

                ipv4->sin_family = AF_INET;
                ipv4->sin_port   = htons(addr.port);

                vr_memory_copy(&ipv4->sin_addr.s_addr,
                    VR_ADDRESS_IP_VER4_SIZE, addr.ip.v4.elements.array);
            } break;

            case VrAddressIpKind_V6: {
                sockaddr_ipv6_t* ipv6 = (sockaddr_ipv6_t*) &result;

                ipv6->sin6_family = AF_INET6;
                ipv6->sin6_port   = htons(addr.port);

                vr_memory_copy(&ipv6->sin6_addr.s6_addr,
                    VR_ADDRESS_IP_VER6_SIZE, addr.ip.v6.elements.array);
            } break;

            default: break;
        }

        return result;
    }

    sockaddr_storage_t vr_linux_sockaddr_make_any(VrAddressIpKind kind, VrUint16 port)
    {
        sockaddr_storage_t result = {0};

        VrUint32 in4addr_any = INADDR_ANY;

        switch (kind) {
            case VrAddressIpKind_V4: {
                sockaddr_ipv4_t* ipv4 = (sockaddr_ipv4_t*) &result;

                ipv4->sin_family = AF_INET;
                ipv4->sin_port   = htons(port);

                vr_memory_copy(&ipv4->sin_addr.s_addr,
                    VR_ADDRESS_IP_VER4_SIZE, (void*) &in4addr_any);
            } break;

            case VrAddressIpKind_V6: {
                sockaddr_ipv6_t* ipv6 = (sockaddr_ipv6_t*) &result;

                ipv6->sin6_family = AF_INET6;
                ipv6->sin6_port   = htons(port);

                vr_memory_copy(&ipv6->sin6_addr.s6_addr,
                    VR_ADDRESS_IP_VER6_SIZE, (void*) &in6addr_any);
            } break;

            default: break;
        }

        return result;
    }

    VrSint vr_linux_sockaddr_size(sockaddr_storage_t* self)
    {
        switch (self->ss_family) {
            case AF_INET:  { return sizeof (sockaddr_ipv4_t); } break;
            case AF_INET6: { return sizeof (sockaddr_ipv6_t); } break;

            default: break;
        }

        return 0;
    }

    VrAddressIp vr_linux_sockaddr_addr(sockaddr_storage_t* self)
    {
        VrAddressIp result = vr_address_ip_none();

        switch (self->ss_family) {
            case AF_INET: {
                sockaddr_ipv4_t* ipv4 = (sockaddr_ipv4_t*) self;

                result.kind = VrAddressIpKind_V4;
                result.port = ntohs(ipv4->sin_port);

                vr_memory_copy(result.ip.v4.elements.array,
                    VR_ADDRESS_IP_VER4_SIZE, &ipv4->sin_addr.s_addr);
            } break;

            case AF_INET6: {
                sockaddr_ipv6_t* ipv6 = (sockaddr_ipv6_t*) self;

                result.kind = VrAddressIpKind_V6;
                result.port = ntohs(ipv6->sin6_port);

                vr_memory_copy(result.ip.v6.elements.array,
                    VR_ADDRESS_IP_VER6_SIZE, &ipv6->sin6_addr.s6_addr);
            } break;

            default: break;
        }

        return result;
    }

    VrLinuxTcpListener* vr_linux_tcp_listener_reserve(VrAlloc* alloc)
    {
        return vr_alloc_reserve_of(alloc, 1, VrLinuxTcpListener);
    }

    VrBool32 vr_linux_tcp_listener_init(VrLinuxTcpListener* self, VrAddressIp addr)
    {
        self->handle  = -1;
        self->queue   = NULL;
        self->address = (sockaddr_storage_t) {0};

        sockaddr_storage_t address = vr_linux_sockaddr_make(addr);
        socklen_t          length  = vr_linux_sockaddr_size(&address);

        if (length <= 0) return 0;

        int flag_ops = SOCK_STREAM | SOCK_CLOEXEC;
        int handle   = socket(address.ss_family, flag_ops, IPPROTO_TCP);

        if (handle == -1) return 0;

        int status = 0;

        do {
            int option = 1;

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

    void vr_linux_tcp_listener_uninit(VrLinuxTcpListener* self)
    {
        if (self->handle != -1) {
            int status = 0;

            do {
                status = close(self->handle);
            }
            while (status == -1 && errno == EINTR);
        }

        self->handle  = -1;
        self->queue   = NULL;
        self->address = (sockaddr_storage_t) {0};
    }

    VrBool32 vr_linux_tcp_listener_bind(VrLinuxTcpListener* self)
    {
        socklen_t length = vr_linux_sockaddr_size(&self->address);

        if (length <= 0) return 0;

        int status = 0;

        do {
            status = bind(self->handle,
                (sockaddr_t*) &self->address, length);
        }
        while (status == -1 && errno == EINTR);

        return status != -1 ? 1 : 0;
    }

    VrBool32 vr_linux_tcp_listener_listen(VrLinuxTcpListener* listener)
    {
        int status = 0;

        do {
            status = listen(listener->handle, SOMAXCONN);
        }
        while (status == -1 && errno == EINTR);

        return status != -1 ? 1 : 0;
    }

    VrAddressIp vr_linux_tcp_listener_addr(VrLinuxTcpListener* self)
    {
        return vr_linux_sockaddr_addr(&self->address);
    }

    #define _vr_tcp_listener_reserve_ vr_linux_tcp_listener_reserve
    #define _vr_tcp_listener_init_    vr_linux_tcp_listener_init
    #define _vr_tcp_listener_uninit_  vr_linux_tcp_listener_uninit
    #define _vr_tcp_listener_bind_    vr_linux_tcp_listener_bind
    #define _vr_tcp_listener_listen_  vr_linux_tcp_listener_listen
    #define _vr_tcp_listener_addr_    vr_linux_tcp_listener_addr

    VrLinuxTcpSocket* vr_linux_tcp_socket_reserve(VrAlloc* alloc)
    {
        return vr_alloc_reserve_of(alloc, 1, VrLinuxTcpSocket);
    }

    VrBool32 vr_linux_tcp_socket_init(VrLinuxTcpSocket* self, VrAddressIp addr)
    {
        self->handle  = -1;
        self->queue   = NULL;
        self->address = (sockaddr_storage_t) {0};

        sockaddr_storage_t address = vr_linux_sockaddr_make(addr);
        socklen_t          length  = vr_linux_sockaddr_size(&address);

        if (length <= 0) return 0;

        int flag_ops = SOCK_STREAM | SOCK_CLOEXEC;
        int handle   = socket(address.ss_family, flag_ops, IPPROTO_TCP);

        if (handle == -1) return 0;

        int status = 0;

        do {
            int option = 1;

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

    void vr_linux_tcp_socket_uninit(VrLinuxTcpSocket* self)
    {
        if (self->handle != -1) {
            int status = 0;

            do {
                status = close(self->handle);
            }
            while (status == -1 && errno == EINTR);
        }

        self->handle  = -1;
        self->queue   = NULL;
        self->address = (sockaddr_storage_t) {0};
    }

    VrBool32 vr_linux_tcp_socket_bind(VrLinuxTcpSocket* self)
    {
        socklen_t length = vr_linux_sockaddr_size(&self->address);

        if (length <= 0) return 0;

        int status = 0;

        do {
            status = bind(self->handle,
                (sockaddr_t*) &self->address, length);
        }
        while (status == -1 && errno == EINTR);

        return status != -1 ? 1 : 0;
    }

    VrBool32 vr_linux_tcp_socket_accept(VrLinuxTcpSocket* self, VrLinuxTcpListener* listener)
    {
        self->handle  = -1;
        self->address = (sockaddr_storage_t) {0};

        sockaddr_storage_t address = {0};
        socklen_t          length  = sizeof address;

        int handle = 0;

        do {
            handle = accept(listener->handle,
                (sockaddr_t*) &address, &length);
        }
        while (handle == -1 && errno == EINTR);

        if (handle != -1) {
            self->handle  = handle;
            self->address = address;

            return 1;
        }

        return 0;
    }

    VrBool32 vr_linux_tcp_socket_connect(VrLinuxTcpSocket* self, VrAddressIp addr)
    {
        sockaddr_storage_t address = vr_linux_sockaddr_make(addr);
        socklen_t          length  = vr_linux_sockaddr_size(&address);

        if (length <= 0) return 0;

        int status = 0;

        do {
            status = connect(self->handle, (sockaddr_t*) &address, length);
        }
        while (status == -1 && errno == EINTR);

        return status != -1 ? 1 : 0;
    }

    VrSint vr_linux_tcp_socket_write(VrLinuxTcpSocket* self, VrUint8* pntr, VrSint size)
    {
        if (pntr == NULL || size <= 0) return 0;

        int count = 0;

        do {
            count = send(self->handle, (VrChar8*) pntr, (VrSint32) size, 0);
        }
        while (count == -1 && errno == EINTR);

        if (count <= 0 || count > size) return 0;

        return count;
    }

    VrSint vr_linux_tcp_socket_read(VrLinuxTcpSocket* self, VrUint8* pntr, VrSint size)
    {
        if (pntr == NULL || size <= 0) return 0;

        int count = 0;

        do {
            count = recv(self->handle, (VrChar8*) pntr, (VrSint32) size, 0);
        }
        while (count == -1 && errno == EINTR);

        if (count <= 0 || count > size) return 0;

        return count;
    }

    VrAddressIp vr_linux_tcp_socket_addr(VrLinuxTcpSocket* self)
    {
        return vr_linux_sockaddr_addr(&self->address);
    }

    #define _vr_tcp_socket_reserve_ vr_linux_tcp_socket_reserve
    #define _vr_tcp_socket_init_    vr_linux_tcp_socket_init
    #define _vr_tcp_socket_uninit_  vr_linux_tcp_socket_uninit
    #define _vr_tcp_socket_bind_    vr_linux_tcp_socket_bind
    #define _vr_tcp_socket_accept_  vr_linux_tcp_socket_accept
    #define _vr_tcp_socket_connect_ vr_linux_tcp_socket_connect
    #define _vr_tcp_socket_write_   vr_linux_tcp_socket_write
    #define _vr_tcp_socket_read_    vr_linux_tcp_socket_read
    #define _vr_tcp_socket_addr_    vr_linux_tcp_socket_addr

    VrLinuxUdpSocket* vr_linux_udp_socket_reserve(VrAlloc* alloc)
    {
        return vr_alloc_reserve_of(alloc, 1, VrLinuxUdpSocket);
    }

    VrBool32 vr_linux_udp_socket_init(VrLinuxUdpSocket* self, VrAddressIp addr)
    {
        self->handle  = -1;
        self->queue   = NULL;
        self->address = (sockaddr_storage_t) {0};

        sockaddr_storage_t address = vr_linux_sockaddr_make(addr);
        socklen_t          length  = vr_linux_sockaddr_size(&address);

        if (length <= 0) return 0;

        int flag_ops = SOCK_DGRAM | SOCK_CLOEXEC;
        int handle   = socket(address.ss_family, flag_ops, IPPROTO_UDP);

        if (handle == -1) return 0;

        int status = 0;

        do {
            int option = 1;

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

    void vr_linux_udp_socket_uninit(VrLinuxUdpSocket* self)
    {
        if (self->handle != -1 ) {
            int status = 0;

            do {
                status = close(self->handle);
            }
            while (status == -1 && errno == EINTR);
        }

        self->handle  = -1;
        self->queue   = NULL;
        self->address = (sockaddr_storage_t) {0};
    }

    VrBool32 vr_linux_udp_socket_bind(VrLinuxUdpSocket* self)
    {
        socklen_t length = vr_linux_sockaddr_size(&self->address);

        if (length <= 0) return 0;

        int status = 0;

        do {
            status = bind(self->handle,
                (sockaddr_t*) &self->address, length);
        }
        while (status == -1 && errno == EINTR);

        return status != -1 ? 1 : 0;
    }

    VrSint vr_linux_udp_socket_write(VrLinuxUdpSocket* self, VrUint8* pntr, VrSint size, VrAddressIp addr)
    {
        if (pntr == NULL || size <= 0) return 0;

        sockaddr_storage_t address = vr_linux_sockaddr_make(addr);
        socklen_t          length  = vr_linux_sockaddr_size(&address);

        int count = 0;

        do {
            count = sendto(self->handle, (VrChar8*) pntr,
                (VrSint32) size, 0, (sockaddr_t*) &address, length);
        }
        while (count == -1 && errno == EINTR);

        if (count <= 0 || count > size) return 0;

        return count;
    }

    VrSint vr_linux_udp_socket_read(VrLinuxUdpSocket* self, VrUint8* pntr, VrSint size, VrAddressIp* addr)
    {
        if (pntr == NULL || size <= 0) return 0;

        sockaddr_storage_t address = {0};
        socklen_t          length  = sizeof address;

        int count = 0;

        do {
            count = recvfrom(self->handle, (VrChar8*) pntr,
                (VrSint32) size, 0, (sockaddr_t*) &address, &length);
        }
        while (count == -1 && errno == EINTR);

        if (count <= 0 || count > size) return 0;

        if (addr != NULL)
            *addr = vr_linux_sockaddr_addr(&address);

        return count;
    }

    VrAddressIp vr_linux_udp_socket_addr(VrLinuxUdpSocket* self)
    {
        return vr_linux_sockaddr_addr(&self->address);
    }

    #define _vr_udp_socket_reserve_ vr_linux_udp_socket_reserve
    #define _vr_udp_socket_init_    vr_linux_udp_socket_init
    #define _vr_udp_socket_uninit_  vr_linux_udp_socket_uninit
    #define _vr_udp_socket_bind_    vr_linux_udp_socket_bind
    #define _vr_udp_socket_write_   vr_linux_udp_socket_write
    #define _vr_udp_socket_read_    vr_linux_udp_socket_read
    #define _vr_udp_socket_addr_    vr_linux_udp_socket_addr

#else

    #error "Unknown platform"

#endif

VrAddressIp vr_address_ip_none(void)
{
    return (VrAddressIp) {.kind = VrAddressIpKind_None};
}

VrAddressIp vr_address_ip_empty(VrAddressIpKind kind)
{
    return (VrAddressIp) {.kind = kind};
}

VrAddressIp vr_address_ip_local(VrAddressIpKind kind, VrUint16 port)
{
    switch (kind) {
        case VrAddressIpKind_V4: {
            VrAddressIpV4 ipv4 = VR_ADDRESS_IP_VER4_LOCAL;

            return (VrAddressIp) {
                .kind = kind,
                .ip   = {.v4 = ipv4},
                .port = port,
            };
        } break;

        case VrAddressIpKind_V6: {
            VrAddressIpV6 ipv6 = VR_ADDRESS_IP_VER6_LOCAL;

            return (VrAddressIp) {
                .kind = kind,
                .ip   = {.v6 = ipv6},
                .port = port,
            };
        } break;

        default: break;
    }

    return (VrAddressIp) {.kind = VrAddressIpKind_None};
}

VrAddressIp vr_address_ipv4(VrAddressIpV4 ip4, VrUint16 port)
{
    return (VrAddressIp) {
        .kind = VrAddressIpKind_V4,
        .ip   = {.v4 = ip4},
        .port = port,
    };
}

VrAddressIp vr_address_ipv6(VrAddressIpV6 ip6, VrUint16 port)
{
    return (VrAddressIp) {
        .kind = VrAddressIpKind_V6,
        .ip   = {.v6 = ip6},
        .port = port,
    };
}

VrBool32 vr_address_ip_is_equal(VrAddressIp self, VrAddressIp other)
{
    if (self.kind != other.kind || self.port != other.port) return 0;

    switch (self.kind) {
        case VrAddressIpKind_V4: {
            for (VrSint i = 0; i < VR_ADDRESS_IP_VER4_SIZE; i += 1) {
                if (self.ip.v4.elements.array[i] != other.ip.v4.elements.array[i])
                    return 0;
            }
        } break;

        case VrAddressIpKind_V6: {
            for (VrSint i = 0; i < VR_ADDRESS_IP_VER6_SIZE; i += 1) {
                if (self.ip.v6.elements.array[i] != other.ip.v6.elements.array[i])
                    return 0;
            }
        } break;

        default: { return 0; } break;
    }

    return 1;
}

VrTcpListener vr_tcp_listener_reserve(VrAlloc* alloc)
{
    return (VrTcpListener) {
        .impl = _vr_tcp_listener_reserve_(alloc),
    };
}

VrBool32 vr_tcp_listener_init(VrTcpListener self, VrAddressIp addr)
{
    return _vr_tcp_listener_init_(self.impl, addr);
}

void vr_tcp_listener_uninit(VrTcpListener self)
{
    _vr_tcp_listener_uninit_(self.impl);
}

VrBool32 vr_tcp_listener_bind_and_listen(VrTcpListener self)
{
    VrBool32 status = 1;

    status &= _vr_tcp_listener_bind_(self.impl);
    status &= _vr_tcp_listener_listen_(self.impl);

    return status;
}

VrAddressIp vr_tcp_listener_addr(VrTcpListener self)
{
    return _vr_tcp_listener_addr_(self.impl);
}

VrTcpSocket vr_tcp_socket_reserve(VrAlloc* alloc)
{
    return (VrTcpSocket) {
        .impl = _vr_tcp_socket_reserve_(alloc),
    };
}

VrBool32 vr_tcp_socket_init(VrTcpSocket self, VrAddressIp addr)
{
    return _vr_tcp_socket_init_(self.impl, addr);
}

void vr_tcp_socket_uninit(VrTcpSocket self)
{
    _vr_tcp_socket_uninit_(self.impl);
}

VrBool32 vr_tcp_socket_bind(VrTcpSocket self)
{
    return _vr_tcp_socket_bind_(self.impl);
}

VrBool32 vr_tcp_socket_accept(VrTcpSocket self, VrTcpListener listener)
{
    return _vr_tcp_socket_accept_(self.impl, listener.impl);
}

VrBool32 vr_tcp_socket_connect(VrTcpSocket self, VrAddressIp addr)
{
    return _vr_tcp_socket_connect_(self.impl, addr);
}

VrSint vr_tcp_socket_write(VrTcpSocket self, VrUint8* pntr, VrSint size)
{
    return _vr_tcp_socket_write_(self.impl, pntr, size);
}

VrSint vr_tcp_socket_write_all(VrTcpSocket self, VrUint8* pntr, VrSint size)
{
    VrSint result = 0;

    for (VrSint count = 0; result < size; result += count) {
        count = _vr_tcp_socket_write_(self.impl,
            pntr + count, size - count);

        if (count < 0 || count >= size) break;
    }

    return result;
}

VrSint vr_tcp_socket_read(VrTcpSocket self, VrUint8* pntr, VrSint size)
{
    return _vr_tcp_socket_read_(self.impl, pntr, size);
}

VrAddressIp vr_tcp_socket_addr(VrTcpSocket self)
{
    return _vr_tcp_socket_addr_(self.impl);
}

VrUdpSocket vr_udp_socket_reserve(VrAlloc* alloc)
{
    return (VrUdpSocket) {
        .impl = _vr_udp_socket_reserve_(alloc),
    };
}

VrBool32 vr_udp_socket_init(VrUdpSocket self, VrAddressIp addr)
{
    return _vr_udp_socket_init_(self.impl, addr);
}

void vr_udp_socket_uninit(VrUdpSocket self)
{
    _vr_udp_socket_uninit_(self.impl);
}

VrBool32 vr_udp_socket_bind(VrUdpSocket self)
{
    return _vr_udp_socket_bind_(self.impl);
}

VrSint vr_udp_socket_write(VrUdpSocket self, VrUint8* pntr, VrSint size, VrAddressIp addr)
{
    return _vr_udp_socket_write_(self.impl, pntr, size, addr);
}

VrSint vr_udp_socket_write_all(VrUdpSocket self, VrUint8* pntr, VrSint size, VrAddressIp addr)
{
    VrSint result = 0;

    for (VrSint count = 0; result < size; result += count) {
        count = _vr_udp_socket_write_(self.impl,
            pntr + count, size - count, addr);

        if (count < 0 || count >= size) break;
    }

    return result;
}

VrSint vr_udp_socket_read(VrUdpSocket self, VrUint8* pntr, VrSint size, VrAddressIp* addr)
{
    return _vr_udp_socket_read_(self.impl, pntr, size, addr);
}

VrAddressIp vr_udp_socket_addr(VrUdpSocket self)
{
    return _vr_udp_socket_addr_(self.impl);
}

#endif
