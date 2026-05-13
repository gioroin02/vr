#ifndef VR_SYSTEM_SOCKET_C
#define VR_SYSTEM_SOCKET_C

#define VR_SYSTEM_SOCKET_IMPL
#include "vr_system_socket.h"

#if VR_SYSTEM == VR_SYSTEM_WINDOWS

    LPFN_CONNECTEX WSAConnectEx = (LPFN_CONNECTEX) NULL;
    LPFN_ACCEPTEX  WSAAcceptEx  = (LPFN_ACCEPTEX)  NULL;

    static volatile long vr_win32_socket_refs = 0;

    static void* _vr_win32_socket_load_(SOCKET handle, GUID guid)
    {
        void* result = NULL;
        DWORD bytes  = 0;

        int32 error = WSAIoctl(handle, SIO_GET_EXTENSION_FUNCTION_POINTER,
            &guid, sizeof guid, &result, sizeof result, &bytes, 0, 0);

        if (error == SOCKET_ERROR) return NULL;

        return result;
    }

    static bool32 _vr_win32_socket_start_()
    {
        WSADATA data = {0};

        if (WSAStartup(MAKEWORD(2, 2), &data) != 0) return 0;

        int32 flag_handle = WSA_FLAG_OVERLAPPED;
        int32 flag_ops    = SOCK_STREAM;

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
            int32 refs_curr = vr_win32_socket_refs;
            int32 refs_new  = refs_curr + 1;

            if (refs_curr == VR_UINT16_MAX) return 0;

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
            int32 refs_curr = vr_win32_socket_refs;
            int32 refs_new  = refs_curr - 1;

            if (refs_curr == VR_UINT16_MIN) break;

            if (InterlockedCompareExchange(&vr_win32_socket_refs, refs_new, refs_curr) == refs_curr) {
                if (refs_new == 0)
                    _vr_win32_socket_stop_();

                break;
            }
        }
    }

    sockaddr_storage_t vr_win32_sockaddr_make(VR_NetworkIpAddr addr)
    {
        sockaddr_storage_t result = {0};

        switch (addr.kind) {
            case VR_NetworkIpAddr_Kind_Ver4: {
                sockaddr_ip_ver4_t* ip_ver4 = (sockaddr_ip_ver4_t*) &result;

                ip_ver4->sin_family = AF_INET;
                ip_ver4->sin_port   = htons(addr.port);

                vr_memory_copy(&ip_ver4->sin_addr.s_addr,
                    VR_NETWORK_IP_ADDR_VER4_SIZE, addr.ip_ver4.elements);
            } break;

            case VR_NetworkIpAddr_Kind_Ver6: {
                sockaddr_ip_ver6_t* ip_ver6 = (sockaddr_ip_ver6_t*) &result;

                ip_ver6->sin6_family = AF_INET6;
                ip_ver6->sin6_port   = htons(addr.port);

                vr_memory_copy(&ip_ver6->sin6_addr.s6_addr,
                    VR_NETWORK_IP_ADDR_VER6_SIZE, addr.ip_ver6.elements);
            } break;

            default: break;
        }

        return result;
    }

    sockaddr_storage_t vr_win32_sockaddr_make_any(VR_NetworkIpAddr_Kind kind, uint16 port)
    {
        sockaddr_storage_t result = {0};

        uint32 in4addr_any = INADDR_ANY;

        switch (kind) {
            case VR_NetworkIpAddr_Kind_Ver4: {
                sockaddr_ip_ver4_t* ip_ver4 = (sockaddr_ip_ver4_t*) &result;

                ip_ver4->sin_family = AF_INET;
                ip_ver4->sin_port   = htons(port);

                vr_memory_copy(&ip_ver4->sin_addr.s_addr,
                    VR_NETWORK_IP_ADDR_VER4_SIZE, (void*) &in4addr_any);
            } break;

            case VR_NetworkIpAddr_Kind_Ver6: {
                sockaddr_ip_ver6_t* ip_ver6 = (sockaddr_ip_ver6_t*) &result;

                ip_ver6->sin6_family = AF_INET6;
                ip_ver6->sin6_port   = htons(port);

                vr_memory_copy(&ip_ver6->sin6_addr.s6_addr,
                    VR_NETWORK_IP_ADDR_VER6_SIZE, (void*) &in6addr_any);
            } break;

            default: break;
        }

        return result;
    }

    intptr vr_win32_sockaddr_size(sockaddr_storage_t* self)
    {
        switch (self->ss_family) {
            case AF_INET:  { return sizeof (sockaddr_ip_ver4_t); } break;
            case AF_INET6: { return sizeof (sockaddr_ip_ver6_t); } break;

            default: break;
        }

        return 0;
    }

    VR_NetworkIpAddr vr_win32_sockaddr_addr(sockaddr_storage_t* self)
    {
        VR_NetworkIpAddr result = vr_network_ip_addr_none();

        switch (self->ss_family) {
            case AF_INET: {
                sockaddr_ip_ver4_t* ip_ver4 = (sockaddr_ip_ver4_t*) self;

                result.kind = VR_NetworkIpAddr_Kind_Ver4;
                result.port = ntohs(ip_ver4->sin_port);

                vr_memory_copy(result.ip_ver4.elements,
                    VR_NETWORK_IP_ADDR_VER4_SIZE, &ip_ver4->sin_addr.s_addr);
            } break;

            case AF_INET6: {
                sockaddr_ip_ver6_t* ip_ver6 = (sockaddr_ip_ver6_t*) self;

                result.kind = VR_NetworkIpAddr_Kind_Ver6;
                result.port = ntohs(ip_ver6->sin6_port);

                vr_memory_copy(result.ip_ver6.elements,
                    VR_NETWORK_IP_ADDR_VER6_SIZE, &ip_ver6->sin6_addr.s6_addr);
            } break;

            default: break;
        }

        return result;
    }

    VR_Win32_SocketTcp* vr_win32_socket_tcp_reserve(VR_Alloc* alloc)
    {
        return vr_alloc_reserve_of(alloc, 1, VR_Win32_SocketTcp);
    }

    bool32 vr_win32_socket_tcp_init(VR_Win32_SocketTcp* self, VR_NetworkIpAddr addr)
    {
        self->handle  = INVALID_SOCKET;
        self->queue   = NULL;
        self->address = (sockaddr_storage_t) {0};

        sockaddr_storage_t address = vr_win32_sockaddr_make(addr);
        socklen_t          length  = vr_win32_sockaddr_size(&address);

        if (length <= 0 || vr_win32_socket_start() == 0) return 0;

        int32 flag_handle = WSA_FLAG_OVERLAPPED;
        int32 flag_ops    = SOCK_STREAM;

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

    void vr_win32_socket_tcp_uninit(VR_Win32_SocketTcp* self)
    {
        if (self->handle == INVALID_SOCKET) return;

        closesocket(self->handle);

        vr_win32_socket_stop();

        self->handle  = INVALID_SOCKET;
        self->queue   = NULL;
        self->address = (sockaddr_storage_t) {0};
    }

    bool32 vr_win32_socket_tcp_bind(VR_Win32_SocketTcp* self)
    {
        sockaddr_storage_t address = self->address;
        socklen_t          length  = vr_win32_sockaddr_size(&address);

        if (length <= 0) return 0;

        if (bind(self->handle, (sockaddr_t*) &address, length) == SOCKET_ERROR)
            return 0;

        return 1;
    }

    bool32 vr_win32_socket_tcp_listen(VR_Win32_SocketTcp* listener)
    {
        if (listen(listener->handle, SOMAXCONN) == SOCKET_ERROR)
            return 0;

        return 1;
    }

    bool32 vr_win32_socket_tcp_accept(VR_Win32_SocketTcp* self, VR_Win32_SocketTcp* listener)
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

    bool32 vr_win32_socket_tcp_connect(VR_Win32_SocketTcp* self, VR_NetworkIpAddr addr)
    {
        sockaddr_storage_t address = vr_win32_sockaddr_make(addr);
        socklen_t          length  = vr_win32_sockaddr_size(&address);

        if (length <= 0) return 0;

        if (connect(self->handle, (sockaddr_t*) &address, length) == SOCKET_ERROR)
            return 0;

        return 1;
    }

    intptr vr_win32_socket_tcp_write(VR_Win32_SocketTcp* self, uint8* pntr, intptr size)
    {
        if (pntr == NULL || size <= 0) return 0;

        int32 count = send(self->handle,
            (char8*) pntr, (int32) size, 0);

        if (count <= 0 || count > size) return 0;

        return count;
    }

    intptr vr_win32_socket_tcp_read(VR_Win32_SocketTcp* self, uint8* pntr, intptr size)
    {
        if (pntr == NULL || size <= 0) return 0;

        int32 count = recv(self->handle,
            (char8*) pntr, (int32) size, 0);

        if (count <= 0 || count > size) return 0;

        return count;
    }

    VR_NetworkIpAddr vr_win32_socket_tcp_addr(VR_Win32_SocketTcp* self)
    {
        return vr_win32_sockaddr_addr(&self->address);
    }

    #define _vr_socket_tcp_reserve_ vr_win32_socket_tcp_reserve
    #define _vr_socket_tcp_init_    vr_win32_socket_tcp_init
    #define _vr_socket_tcp_uninit_  vr_win32_socket_tcp_uninit
    #define _vr_socket_tcp_bind_    vr_win32_socket_tcp_bind
    #define _vr_socket_tcp_listen_  vr_win32_socket_tcp_listen
    #define _vr_socket_tcp_accept_  vr_win32_socket_tcp_accept
    #define _vr_socket_tcp_connect_ vr_win32_socket_tcp_connect
    #define _vr_socket_tcp_write_   vr_win32_socket_tcp_write
    #define _vr_socket_tcp_read_    vr_win32_socket_tcp_read
    #define _vr_socket_tcp_addr_    vr_win32_socket_tcp_addr

    VR_Win32_SocketUdp* vr_win32_socket_udp_reserve(VR_Alloc* alloc)
    {
        return vr_alloc_reserve_of(alloc, 1, VR_Win32_SocketUdp);
    }

    bool32 vr_win32_socket_udp_init(VR_Win32_SocketUdp* self, VR_NetworkIpAddr addr)
    {
        self->handle  = INVALID_SOCKET;
        self->queue   = NULL;
        self->address = (sockaddr_storage_t) {0};

        sockaddr_storage_t address = vr_win32_sockaddr_make(addr);
        socklen_t          length  = vr_win32_sockaddr_size(&address);

        if (length <= 0 || vr_win32_socket_start() == 0) return 0;

        int32 flag_handle = WSA_FLAG_OVERLAPPED;
        int32 flag_ops    = SOCK_DGRAM;

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

    void vr_win32_socket_udp_uninit(VR_Win32_SocketUdp* self)
    {
        if (self->handle == INVALID_SOCKET) return;

        closesocket(self->handle);

        vr_win32_socket_stop();

        self->handle  = INVALID_SOCKET;
        self->queue   = NULL;
        self->address = (sockaddr_storage_t) {0};
    }

    bool32 vr_win32_socket_udp_bind(VR_Win32_SocketUdp* self)
    {
        sockaddr_storage_t address = self->address;
        socklen_t          length  = vr_win32_sockaddr_size(&address);

        if (length <= 0) return 0;

        if (bind(self->handle, (sockaddr_t*) &address, length) == SOCKET_ERROR)
            return 0;

        return 1;
    }

    intptr vr_win32_socket_udp_write(VR_Win32_SocketUdp* self, uint8* pntr, intptr size, VR_NetworkIpAddr addr)
    {
        if (pntr == NULL || size <= 0) return 0;

        sockaddr_storage_t address = vr_win32_sockaddr_make(addr);
        socklen_t          length  = vr_win32_sockaddr_size(&address);

        int32 count = sendto(self->handle, (char8*) pntr,
            (int32) size, 0, (sockaddr_t*) &address, length);

        if (count <= 0 || count > size) return 0;

        return count;
    }

    intptr vr_win32_socket_udp_read(VR_Win32_SocketUdp* self, uint8* pntr, intptr size, VR_NetworkIpAddr* addr)
    {
        if (pntr == NULL || size <= 0) return 0;

        sockaddr_storage_t address = {0};
        socklen_t          length  = sizeof address;

        int32 count = recvfrom(self->handle, (char8*) pntr,
            (int32) size, 0, (sockaddr_t*) &address, &length);

        if (count <= 0 || count > size) return 0;

        if (addr != NULL)
            *addr = vr_win32_sockaddr_addr(&address);

        return count;
    }

    VR_NetworkIpAddr vr_win32_socket_udp_addr(VR_Win32_SocketUdp* self)
    {
        return vr_win32_sockaddr_addr(&self->address);
    }

    #define _vr_socket_udp_reserve_ vr_win32_socket_udp_reserve
    #define _vr_socket_udp_init_    vr_win32_socket_udp_init
    #define _vr_socket_udp_uninit_  vr_win32_socket_udp_uninit
    #define _vr_socket_udp_bind_    vr_win32_socket_udp_bind
    #define _vr_socket_udp_write_   vr_win32_socket_udp_write
    #define _vr_socket_udp_read_    vr_win32_socket_udp_read
    #define _vr_socket_udp_addr_    vr_win32_socket_tcp_addr

#elif VR_SYSTEM == VR_SYSTEM_LINUX

    sockaddr_storage_t vr_linux_sockaddr_make(VR_NetworkIpAddr addr)
    {
        sockaddr_storage_t result = {0};

        switch (addr.kind) {
            case VR_NetworkIpAddr_Kind_Ver4: {
                sockaddr_ip_ver4_t* ip_ver4 = (sockaddr_ip_ver4_t*) &result;

                ip_ver4->sin_family = AF_INET;
                ip_ver4->sin_port   = htons(addr.port);

                vr_memory_copy(&ip_ver4->sin_addr.s_addr,
                    VR_NETWORK_IP_ADDR_VER4_SIZE, addr.ip_ver4.elements);
            } break;

            case VR_NetworkIpAddr_Kind_Ver6: {
                sockaddr_ip_ver6_t* ip_ver6 = (sockaddr_ip_ver6_t*) &result;

                ip_ver6->sin6_family = AF_INET6;
                ip_ver6->sin6_port   = htons(addr.port);

                vr_memory_copy(&ip_ver6->sin6_addr.s6_addr,
                    VR_NETWORK_IP_ADDR_VER6_SIZE, addr.ip_ver6.elements);
            } break;

            default: break;
        }

        return result;
    }

    sockaddr_storage_t vr_linux_sockaddr_make_any(VR_NetworkIpAddr_Kind kind, uint16 port)
    {
        sockaddr_storage_t result = {0};

        uint32 in4addr_any = INADDR_ANY;

        switch (kind) {
            case VR_NetworkIpAddr_Kind_Ver4: {
                sockaddr_ip_ver4_t* ip_ver4 = (sockaddr_ip_ver4_t*) &result;

                ip_ver4->sin_family = AF_INET;
                ip_ver4->sin_port   = htons(port);

                vr_memory_copy(&ip_ver4->sin_addr.s_addr,
                    VR_NETWORK_IP_ADDR_VER4_SIZE, (void*) &in4addr_any);
            } break;

            case VR_NetworkIpAddr_Kind_Ver6: {
                sockaddr_ip_ver6_t* ip_ver6 = (sockaddr_ip_ver6_t*) &result;

                ip_ver6->sin6_family = AF_INET6;
                ip_ver6->sin6_port   = htons(port);

                vr_memory_copy(&ip_ver6->sin6_addr.s6_addr,
                    VR_NETWORK_IP_ADDR_VER6_SIZE, (void*) &in6addr_any);
            } break;

            default: break;
        }

        return result;
    }

    intptr vr_linux_sockaddr_size(sockaddr_storage_t* self)
    {
        switch (self->ss_family) {
            case AF_INET:  { return sizeof (sockaddr_ip_ver4_t); } break;
            case AF_INET6: { return sizeof (sockaddr_ip_ver6_t); } break;

            default: break;
        }

        return 0;
    }

    VR_NetworkIpAddr vr_linux_sockaddr_addr(sockaddr_storage_t* self)
    {
        VR_NetworkIpAddr result = vr_network_ip_addr_none();

        switch (self->ss_family) {
            case AF_INET: {
                sockaddr_ip_ver4_t* ip_ver4 = (sockaddr_ip_ver4_t*) self;

                result.kind = VR_NetworkIpAddr_Kind_Ver4;
                result.port = ntohs(ip_ver4->sin_port);

                vr_memory_copy(result.ip_ver4.elements,
                    VR_NETWORK_IP_ADDR_VER4_SIZE, &ip_ver4->sin_addr.s_addr);
            } break;

            case AF_INET6: {
                sockaddr_ip_ver6_t* ip_ver6 = (sockaddr_ip_ver6_t*) self;

                result.kind = VR_NetworkIpAddr_Kind_Ver6;
                result.port = ntohs(ip_ver6->sin6_port);

                vr_memory_copy(result.ip_ver6.elements,
                    VR_NETWORK_IP_ADDR_VER6_SIZE, &ip_ver6->sin6_addr.s6_addr);
            } break;

            default: break;
        }

        return result;
    }

    VR_Linux_SocketTcp* vr_linux_socket_tcp_reserve(VR_Alloc* alloc)
    {
        return vr_alloc_reserve_of(alloc, 1, VR_Linux_SocketTcp);
    }

    bool32 vr_linux_socket_tcp_init(VR_Linux_SocketTcp* self, VR_NetworkIpAddr addr)
    {
        self->handle  = -1;
        self->queue   = NULL;
        self->address = (sockaddr_storage_t) {0};

        sockaddr_storage_t address = vr_linux_sockaddr_make(addr);
        socklen_t          length  = vr_linux_sockaddr_size(&address);

        if (length <= 0) return 0;

        int32 flag_ops = SOCK_STREAM | SOCK_CLOEXEC;
        int32 option   = 1;
        int32 status   = 0;

        int32 handle = socket(address.ss_family, flag_ops, IPPROTO_TCP);

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

    void vr_linux_socket_tcp_uninit(VR_Linux_SocketTcp* self)
    {
        if (self->handle != -1) {
            int32 status = 0;

            do {
                status = close(self->handle);
            }
            while (status == -1 && errno == EINTR);
        }

        self->handle  = -1;
        self->queue   = NULL;
        self->address = (sockaddr_storage_t) {0};
    }

    bool32 vr_linux_socket_tcp_bind(VR_Linux_SocketTcp* self)
    {
        sockaddr_storage_t address = self->address;
        socklen_t          length  = vr_linux_sockaddr_size(&address);
        int32              status  = 0;

        if (length <= 0) return 0;

        do {
            status = bind(self->handle, (sockaddr_t*) &address, length);
        }
        while (status == -1 && errno == EINTR);

        return status != -1 ? 1 : 0;
    }

    bool32 vr_linux_socket_tcp_listen(VR_Linux_SocketTcp* listener)
    {
        int32 status = 0;

        do {
            status = listen(listener->handle, SOMAXCONN);
        }
        while (status == -1 && errno == EINTR);

        return status != -1 ? 1 : 0;
    }

    bool32 vr_linux_socket_tcp_accept(VR_Linux_SocketTcp* self, VR_Linux_SocketTcp* listener)
    {
        self->handle  = -1;
        self->address = (sockaddr_storage_t) {0};

        sockaddr_storage_t address = {0};
        socklen_t          length  = sizeof address;

        int32 handle = 0;

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

    bool32 vr_linux_socket_tcp_connect(VR_Linux_SocketTcp* self, VR_NetworkIpAddr addr)
    {
        sockaddr_storage_t address = vr_linux_sockaddr_make(addr);
        socklen_t          length  = vr_linux_sockaddr_size(&address);
        int32              status  = 0;

        if (length <= 0) return 0;

        do {
            status = connect(self->handle, (sockaddr_t*) &address, length);
        }
        while (status == -1 && errno == EINTR);

        return status != -1 ? 1 : 0;
    }

    intptr vr_linux_socket_tcp_write(VR_Linux_SocketTcp* self, uint8* pntr, intptr size)
    {
        if (pntr == NULL || size <= 0) return 0;

        int32 count = 0;

        do {
            count = send(self->handle,
                (char8*) pntr, (int32) size, 0);
        }
        while (count == -1 && errno == EINTR);

        if (count <= 0 || count > size) return 0;

        return count;
    }

    intptr vr_linux_socket_tcp_read(VR_Linux_SocketTcp* self, uint8* pntr, intptr size)
    {
        if (pntr == NULL || size <= 0) return 0;

        int32 count = 0;

        do {
            count = recv(self->handle,
                (char8*) pntr, (int32) size, 0);
        }
        while (count == -1 && errno == EINTR);

        if (count <= 0 || count > size) return 0;

        return count;
    }

    VR_NetworkIpAddr vr_linux_socket_tcp_addr(VR_Linux_SocketTcp* self)
    {
        return vr_linux_sockaddr_addr(&self->address);
    }

    #define _vr_socket_tcp_reserve_ vr_linux_socket_tcp_reserve
    #define _vr_socket_tcp_init_    vr_linux_socket_tcp_init
    #define _vr_socket_tcp_uninit_  vr_linux_socket_tcp_uninit
    #define _vr_socket_tcp_bind_    vr_linux_socket_tcp_bind
    #define _vr_socket_tcp_listen_  vr_linux_socket_tcp_listen
    #define _vr_socket_tcp_accept_  vr_linux_socket_tcp_accept
    #define _vr_socket_tcp_connect_ vr_linux_socket_tcp_connect
    #define _vr_socket_tcp_write_   vr_linux_socket_tcp_write
    #define _vr_socket_tcp_read_    vr_linux_socket_tcp_read
    #define _vr_socket_tcp_addr_    vr_linux_socket_tcp_addr

    VR_Linux_SocketUdp* vr_linux_socket_udp_reserve(VR_Alloc* alloc)
    {
        return vr_alloc_reserve_of(alloc, 1, VR_Linux_SocketUdp);
    }

    bool32 vr_linux_socket_udp_init(VR_Linux_SocketUdp* self, VR_NetworkIpAddr addr)
    {
        self->handle  = -1;
        self->queue   = NULL;
        self->address = (sockaddr_storage_t) {0};

        sockaddr_storage_t address = vr_linux_sockaddr_make(addr);
        socklen_t          length  = vr_linux_sockaddr_size(&address);

        if (length <= 0) return 0;

        int32 flag_ops = SOCK_DGRAM | SOCK_CLOEXEC;
        int32 option   = 1;
        int32 status   = 0;

        int32 handle = socket(address.ss_family, flag_ops, IPPROTO_UDP);

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

    void vr_linux_socket_udp_uninit(VR_Linux_SocketUdp* self)
    {
        if (self->handle != -1 ) {
            int32 status = 0;

            do {
                status = close(self->handle);
            }
            while (status == -1 && errno == EINTR);
        }

        self->handle  = -1;
        self->queue   = NULL;
        self->address = (sockaddr_storage_t) {0};
    }

    bool32 vr_linux_socket_udp_bind(VR_Linux_SocketUdp* self)
    {
        sockaddr_storage_t address = self->address;
        socklen_t          length  = vr_linux_sockaddr_size(&address);
        int32              status  = 0;

        if (length <= 0) return 0;

        do {
            status = bind(self->handle, (sockaddr_t*) &address, length);
        }
        while (status == -1 && errno == EINTR);

        return status != -1 ? 1 : 0;
    }

    intptr vr_linux_socket_udp_write(VR_Linux_SocketUdp* self, uint8* pntr, intptr size, VR_NetworkIpAddr addr)
    {
        if (pntr == NULL || size <= 0) return 0;

        sockaddr_storage_t address = vr_linux_sockaddr_make(addr);
        socklen_t          length  = vr_linux_sockaddr_size(&address);

        int32 count = 0;

        do {
            count = sendto(self->handle, (char8*) pntr,
                (int32) size, 0, (sockaddr_t*) &address, length);
        }
        while (count == -1 && errno == EINTR);

        if (count <= 0 || count > size) return 0;

        return count;
    }

    intptr vr_linux_socket_udp_read(VR_Linux_SocketUdp* self, uint8* pntr, intptr size, VR_NetworkIpAddr* addr)
    {
        if (pntr == NULL || size <= 0) return 0;

        sockaddr_storage_t address = {0};
        socklen_t          length  = sizeof address;

        int32 count = 0;

        do {
            count = recvfrom(self->handle, (char8*) pntr,
                (int32) size, 0, (sockaddr_t*) &address, &length);
        }
        while (count == -1 && errno == EINTR);

        if (count <= 0 || count > size) return 0;

        if (addr != NULL)
            *addr = vr_linux_sockaddr_addr(&address);

        return count;
    }

    VR_NetworkIpAddr vr_linux_socket_udp_addr(VR_Linux_SocketUdp* self)
    {
        return vr_linux_sockaddr_addr(&self->address);
    }

    #define _vr_socket_udp_reserve_ vr_linux_socket_udp_reserve
    #define _vr_socket_udp_init_    vr_linux_socket_udp_init
    #define _vr_socket_udp_uninit_  vr_linux_socket_udp_uninit
    #define _vr_socket_udp_bind_    vr_linux_socket_udp_bind
    #define _vr_socket_udp_write_   vr_linux_socket_udp_write
    #define _vr_socket_udp_read_    vr_linux_socket_udp_read
    #define _vr_socket_udp_addr_    vr_linux_socket_udp_addr

#else

    #error "Unknown platform"

#endif

VR_NetworkIpAddr vr_network_ip_addr_none()
{
    return (VR_NetworkIpAddr) {.kind = VR_NetworkIpAddr_Kind_None};
}

VR_NetworkIpAddr vr_network_ip_addr_empty(VR_NetworkIpAddr_Kind kind)
{
    return (VR_NetworkIpAddr) {.kind = kind};
}

VR_NetworkIpAddr vr_network_ip_addr_local(VR_NetworkIpAddr_Kind kind, uint16 port)
{
    switch (kind) {
        case VR_NetworkIpAddr_Kind_Ver4: {
            VR_NetworkIpAddr_Ver4 ipv4 = VR_NETWORK_IP_ADDR_VER4_LOCAL;

            return (VR_NetworkIpAddr) {
                .kind    = kind,
                .ip_ver4 = ipv4,
                .port    = port,
            };
        } break;

        case VR_NetworkIpAddr_Kind_Ver6: {
            VR_NetworkIpAddr_Ver6 ipv6 = VR_NETWORK_IP_ADDR_VER6_LOCAL;

            return (VR_NetworkIpAddr) {
                .kind    = kind,
                .ip_ver6 = ipv6,
                .port    = port,
            };
        } break;

        default: break;
    }

    return (VR_NetworkIpAddr) {.kind = VR_NetworkIpAddr_Kind_None};
}

VR_NetworkIpAddr vr_network_ip_addr_ver4(VR_NetworkIpAddr_Ver4 ipv4, uint16 port)
{
    return (VR_NetworkIpAddr) {
        .kind    = VR_NetworkIpAddr_Kind_Ver4,
        .ip_ver4 = ipv4,
        .port    = port,
    };
}

VR_NetworkIpAddr vr_network_ip_addr_ver6(VR_NetworkIpAddr_Ver6 ipv6, uint16 port)
{
    return (VR_NetworkIpAddr) {
        .kind    = VR_NetworkIpAddr_Kind_Ver6,
        .ip_ver6 = ipv6,
        .port    = port,
    };
}

bool32 vr_network_ip_addr_is_equal(VR_NetworkIpAddr self, VR_NetworkIpAddr other)
{
    if (self.kind != other.kind || self.port != other.port)
        return 0;

    switch (self.kind) {
        case VR_NetworkIpAddr_Kind_Ver4: {
            for (intptr i = 0; i < VR_NETWORK_IP_ADDR_VER4_SIZE; i += 1) {
                if (self.ip_ver4.elements[i] != other.ip_ver4.elements[i])
                    return 0;
            }

            return 1;
        } break;

        case VR_NetworkIpAddr_Kind_Ver6: {
            for (intptr i = 0; i < VR_NETWORK_IP_ADDR_VER6_SIZE; i += 1) {
                if (self.ip_ver6.elements[i] != other.ip_ver6.elements[i])
                    return 0;
            }

            return 1;
        } break;

        default: break;
    }

    return 0;
}

VR_SocketTcp vr_socket_tcp_reserve(VR_Alloc* alloc)
{
    return (VR_SocketTcp) {
        .impl = _vr_socket_tcp_reserve_(alloc),
    };
}

bool32 vr_socket_tcp_init(VR_SocketTcp self, VR_NetworkIpAddr_Kind kind)
{
    return _vr_socket_tcp_init_(self.impl, vr_network_ip_addr_empty(kind));
}

bool32 vr_socket_tcp_init_bound(VR_SocketTcp self, VR_NetworkIpAddr_Kind kind, uint16 port)
{
    VR_NetworkIpAddr addr = vr_network_ip_addr_local(kind, port);

    if (addr.kind == VR_NetworkIpAddr_Kind_None && port <= 0)
        return 0;

    bool32 status = 1;

    status &= _vr_socket_tcp_init_(self.impl, addr);
    status &= _vr_socket_tcp_bind_(self.impl);

    if (status == 0)
        _vr_socket_tcp_uninit_(self.impl);

    return status;
}

void vr_socket_tcp_uninit(VR_SocketTcp self)
{
    return _vr_socket_tcp_uninit_(self.impl);
}

bool32 vr_socket_tcp_listen(VR_SocketTcp listener)
{
    return _vr_socket_tcp_listen_(listener.impl);
}

bool32 vr_socket_tcp_accept(VR_SocketTcp self, VR_SocketTcp listener)
{
    return _vr_socket_tcp_accept_(self.impl, listener.impl);
}

bool32 vr_socket_tcp_connect(VR_SocketTcp self, VR_NetworkIpAddr addr)
{
    return _vr_socket_tcp_connect_(self.impl, addr);
}

intptr vr_socket_tcp_write(VR_SocketTcp self, uint8* pntr, intptr size)
{
    return _vr_socket_tcp_write_(self.impl, pntr, size);
}

intptr vr_socket_tcp_write_all(VR_SocketTcp self, uint8* pntr, intptr size)
{
    intptr result = 0;

    for (intptr count = 0; result < size; result += count) {
        count = _vr_socket_tcp_write_(self.impl,
            pntr + count, size - count);

        if (count < 0 || count >= size) break;
    }

    return result;
}

intptr vr_socket_tcp_read(VR_SocketTcp self, uint8* pntr, intptr size)
{
    return _vr_socket_tcp_read_(self.impl, pntr, size);
}

VR_NetworkIpAddr vr_socket_tcp_addr(VR_SocketTcp self)
{
    return _vr_socket_tcp_addr_(self.impl);
}

VR_SocketUdp vr_socket_udp_reserve(VR_Alloc* alloc)
{
    return (VR_SocketUdp) {
        .impl = _vr_socket_udp_reserve_(alloc),
    };
}

bool32 vr_socket_udp_init(VR_SocketUdp self, VR_NetworkIpAddr_Kind kind)
{
    return _vr_socket_udp_init_(self.impl, vr_network_ip_addr_empty(kind));
}

bool32 vr_socket_udp_init_bound(VR_SocketUdp self, VR_NetworkIpAddr_Kind kind, uint16 port)
{
    VR_NetworkIpAddr addr = vr_network_ip_addr_local(kind, port);

    if (addr.kind == VR_NetworkIpAddr_Kind_None && port <= 0)
        return 0;

    bool32 status = 1;

    status &= _vr_socket_udp_init_(self.impl, addr);
    status &= _vr_socket_udp_bind_(self.impl);

    if (status == 0)
        _vr_socket_udp_uninit_(self.impl);

    return status;
}

void vr_socket_udp_uninit(VR_SocketUdp self)
{
    return _vr_socket_udp_uninit_(self.impl);
}

intptr vr_socket_udp_write(VR_SocketUdp self, uint8* pntr, intptr size, VR_NetworkIpAddr addr)
{
    return _vr_socket_udp_write_(self.impl, pntr, size, addr);
}

intptr vr_socket_udp_write_all(VR_SocketUdp self, uint8* pntr, intptr size, VR_NetworkIpAddr addr)
{
    intptr result = 0;

    for (intptr count = 0; result < size; result += count) {
        count = _vr_socket_udp_write_(self.impl,
            pntr + count, size - count, addr);

        if (count < 0 || count >= size) break;
    }

    return result;
}

intptr vr_socket_udp_read(VR_SocketUdp self, uint8* pntr, intptr size, VR_NetworkIpAddr* addr)
{
    return _vr_socket_udp_read_(self.impl, pntr, size, addr);
}

VR_NetworkIpAddr vr_socket_udp_addr(VR_SocketUdp self)
{
    return _vr_socket_udp_addr_(self.impl);
}

#endif
