#ifndef VR_SYSTEM_SOCKET_H
#define VR_SYSTEM_SOCKET_H

#include "vr_base_memory.h"

typedef enum VR_Network_Ip_Addr_Kind
{
    VR_Network_Ip_Addr_Kind_None,
    VR_Network_Ip_Addr_Kind_Ver4,
    VR_Network_Ip_Addr_Kind_Ver6,
}
VR_Network_Ip_Addr_Kind;

#define VR_NETWORK_IP_ADDR_VER4_SIZE  ((intptr) 4)
#define VR_NETWORK_IP_ADDR_VER4_EMPTY ((VR_Network_Ip_Addr_Ver4) {0})
#define VR_NETWORK_IP_ADDR_VER4_LOCAL ((VR_Network_Ip_Addr_Ver4) {.elem_0 = 0x7F, .elem_3 = 0x01})

typedef union VR_Network_Ip_Addr_V4
{
    uint8 elements[VR_NETWORK_IP_ADDR_VER4_SIZE];

    struct {
        uint8 elem_0, elem_1, elem_2, elem_3;
    };
}
VR_Network_Ip_Addr_Ver4;

#define VR_NETWORK_IP_ADDR_VER6_SIZE  ((intptr) 16)
#define VR_NETWORK_IP_ADDR_VER6_EMPTY ((VR_Network_Ip_Addr_Ver6) {0})
#define VR_NETWORK_IP_ADDR_VER6_LOCAL ((VR_Network_Ip_Addr_Ver6) {.elem_15 = 0x01})

typedef union VR_Network_Ip_Addr_Ver6
{
    uint8 elements[VR_NETWORK_IP_ADDR_VER6_SIZE];

    struct {
        uint8 elem_0,  elem_1,  elem_2,  elem_3;
        uint8 elem_4,  elem_5,  elem_6,  elem_7;
        uint8 elem_8,  elem_9,  elem_10, elem_11;
        uint8 elem_12, elem_13, elem_14, elem_15;
    };
}
VR_Network_Ip_Addr_Ver6;

typedef struct VR_Network_Ip_Addr
{
    VR_Network_Ip_Addr_Kind kind;

    union {
        VR_Network_Ip_Addr_Ver4 ip_ver4;
        VR_Network_Ip_Addr_Ver6 ip_ver6;
    };

    uint16 port;
}
VR_Network_Ip_Addr;

VR_Network_Ip_Addr vr_network_ip_addr_none();

VR_Network_Ip_Addr vr_network_ip_addr_empty(VR_Network_Ip_Addr_Kind kind);

VR_Network_Ip_Addr vr_network_ip_addr_local(VR_Network_Ip_Addr_Kind kind, uint16 port);

VR_Network_Ip_Addr vr_network_ip_addr_ver4(VR_Network_Ip_Addr_Ver4 ip4, uint16 port);

VR_Network_Ip_Addr vr_network_ip_addr_ver6(VR_Network_Ip_Addr_Ver6 ip6, uint16 port);

bool32 vr_network_ip_addr_is_equal(VR_Network_Ip_Addr self, VR_Network_Ip_Addr other);

typedef struct VR_Socket_Tcp
{
    void* impl;
}
VR_Socket_Tcp;

VR_Socket_Tcp vr_socket_tcp_reserve(VR_Alloc* alloc);

bool32 vr_socket_tcp_init(VR_Socket_Tcp self, VR_Network_Ip_Addr_Kind kind);

bool32 vr_socket_tcp_init_bound(VR_Socket_Tcp self, VR_Network_Ip_Addr_Kind kind, uint16 port);

bool32 vr_socket_tcp_listen(VR_Socket_Tcp listener);

void vr_socket_tcp_uninit(VR_Socket_Tcp self);

bool32 vr_socket_tcp_accept(VR_Socket_Tcp self, VR_Socket_Tcp listener);

bool32 vr_socket_tcp_connect(VR_Socket_Tcp self, VR_Network_Ip_Addr addr);

intptr vr_socket_tcp_write(VR_Socket_Tcp self, uint8* pntr, intptr size);

intptr vr_socket_tcp_write_all(VR_Socket_Tcp self, uint8* pntr, intptr size);

intptr vr_socket_tcp_read(VR_Socket_Tcp self, uint8* pntr, intptr size);

VR_Network_Ip_Addr vr_socket_tcp_addr(VR_Socket_Tcp self);

typedef struct VR_Socket_Udp
{
    void* impl;
}
VR_Socket_Udp;

VR_Socket_Udp vr_socket_udp_reserve(VR_Alloc* alloc);

bool32 vr_socket_udp_init(VR_Socket_Udp self, VR_Network_Ip_Addr_Kind kind);

bool32 vr_socket_udp_init_bound(VR_Socket_Udp self, VR_Network_Ip_Addr_Kind kind, uint16 port);

void vr_socket_udp_uninit(VR_Socket_Udp self);

intptr vr_socket_udp_write(VR_Socket_Udp self, uint8* pntr, intptr size, VR_Network_Ip_Addr addr);

intptr vr_socket_udp_write_all(VR_Socket_Udp self, uint8* pntr, intptr size, VR_Network_Ip_Addr addr);

intptr vr_socket_udp_read(VR_Socket_Udp self, uint8* pntr, intptr size, VR_Network_Ip_Addr* addr);

VR_Network_Ip_Addr vr_socket_udp_addr(VR_Socket_Udp self);

#if defined VR_SYSTEM_SOCKET_IMPL

    #if VR_SYSTEM == VR_SYSTEM_WINDOWS

        #define WIN32_LEAN_AND_MEAN
        #define NOGDI
        #include <windows.h>

        #include <winsock2.h>
        #include <ws2tcpip.h>
        #include <mswsock.h>

        typedef struct sockaddr_storage sockaddr_storage_t;
        typedef struct sockaddr         sockaddr_t;
        typedef struct sockaddr_in      sockaddr_ip_ver4_t;
        typedef struct sockaddr_in6     sockaddr_ip_ver6_t;

        bool32 vr_win32_socket_start();
        void   vr_win32_socket_stop();

        sockaddr_storage_t vr_win32_sockaddr_make(VR_Network_Ip_Addr addr);

        sockaddr_storage_t vr_win32_sockaddr_make_any(VR_Network_Ip_Addr_Kind kind, uint16 port);

        intptr vr_win32_sockaddr_size(sockaddr_storage_t* self);

        VR_Network_Ip_Addr vr_win32_sockaddr_addr(sockaddr_storage_t* self);

        typedef struct VR_Win32_Socket_Tcp
        {
            SOCKET handle;
            void*  queue;

            sockaddr_storage_t address;
        }
        VR_Win32_Socket_Tcp;

        VR_Win32_Socket_Tcp* vr_win32_socket_tcp_reserve(VR_Alloc* alloc);

        bool32 vr_win32_socket_tcp_init(VR_Win32_Socket_Tcp* self, VR_Network_Ip_Addr addr);

        void vr_win32_socket_tcp_uninit(VR_Win32_Socket_Tcp* self);

        bool32 vr_win32_socket_tcp_bind(VR_Win32_Socket_Tcp* self);

        bool32 vr_win32_socket_tcp_listen(VR_Win32_Socket_Tcp* listener);

        bool32 vr_win32_socket_tcp_accept(VR_Win32_Socket_Tcp* self, VR_Win32_Socket_Tcp* listener);

        bool32 vr_win32_socket_tcp_connect(VR_Win32_Socket_Tcp* self, VR_Network_Ip_Addr addr);

        intptr vr_win32_socket_tcp_write(VR_Win32_Socket_Tcp* self, uint8* pntr, intptr size);

        intptr vr_win32_socket_tcp_read(VR_Win32_Socket_Tcp* self, uint8* pntr, intptr size);

        VR_Network_Ip_Addr vr_win32_socket_tcp_addr(VR_Win32_Socket_Tcp* self);

        typedef struct VR_Win32_Socket_Udp
        {
            SOCKET handle;
            void*  queue;

            sockaddr_storage_t address;
        }
        VR_Win32_Socket_Udp;

        VR_Win32_Socket_Udp* vr_win32_socket_udp_reserve(VR_Alloc* alloc);

        bool32 vr_win32_socket_udp_init(VR_Win32_Socket_Udp* self, VR_Network_Ip_Addr addr);

        void vr_win32_socket_udp_uninit(VR_Win32_Socket_Udp* self);

        bool32 vr_win32_socket_udp_bind(VR_Win32_Socket_Udp* self);

        intptr vr_win32_socket_udp_write(VR_Win32_Socket_Udp* self, uint8* pntr, intptr size, VR_Network_Ip_Addr addr);

        intptr vr_win32_socket_udp_read(VR_Win32_Socket_Udp* self, uint8* pntr, intptr size, VR_Network_Ip_Addr* addr);

        VR_Network_Ip_Addr vr_win32_socket_udp_addr(VR_Win32_Socket_Udp* self);

    #elif VR_SYSTEM == VR_SYSTEM_LINUX

        #include <unistd.h>
        #include <errno.h>

        #include <sys/socket.h>
        #include <netinet/in.h>

        typedef struct sockaddr_storage sockaddr_storage_t;
        typedef struct sockaddr         sockaddr_t;
        typedef struct sockaddr_in      sockaddr_ip_ver4_t;
        typedef struct sockaddr_in6     sockaddr_ip_ver6_t;

        sockaddr_storage_t vr_linux_sockaddr_make(VR_Network_Ip_Addr addr);

        sockaddr_storage_t vr_linux_sockaddr_make_any(VR_Network_Ip_Addr_Kind kind, uint16 port);

        intptr vr_linux_sockaddr_size(sockaddr_storage_t* self);

        VR_Network_Ip_Addr vr_linux_sockaddr_addr(sockaddr_storage_t* self);

        typedef struct VR_Linux_Socket_Tcp
        {
            int32 handle;
            void* queue;

            sockaddr_storage_t address;
        }
        VR_Linux_Socket_Tcp;

        VR_Linux_Socket_Tcp* vr_linux_socket_tcp_reserve(VR_Alloc* alloc);

        bool32 vr_linux_socket_tcp_init(VR_Linux_Socket_Tcp* self, VR_Network_Ip_Addr addr);

        void vr_linux_socket_tcp_uninit(VR_Linux_Socket_Tcp* self);

        bool32 vr_linux_socket_tcp_bind(VR_Linux_Socket_Tcp* self);

        bool32 vr_linux_socket_tcp_listen(VR_Linux_Socket_Tcp* listener);

        bool32 vr_linux_socket_tcp_accept(VR_Linux_Socket_Tcp* self, VR_Linux_Socket_Tcp* listener);

        bool32 vr_linux_socket_tcp_connect(VR_Linux_Socket_Tcp* self, VR_Network_Ip_Addr addr);

        intptr vr_linux_socket_tcp_write(VR_Linux_Socket_Tcp* self, uint8* pntr, intptr size);

        intptr vr_linux_socket_tcp_read(VR_Linux_Socket_Tcp* self, uint8* pntr, intptr size);

        VR_Network_Ip_Addr vr_linux_socket_tcp_addr(VR_Linux_Socket_Tcp* self);

        typedef struct VR_Linux_Socket_Udp
        {
            int32 handle;
            void* queue;

            sockaddr_storage_t address;
        }
        VR_Linux_Socket_Udp;

        VR_Linux_Socket_Udp* vr_linux_socket_udp_reserve(VR_Alloc* alloc);

        bool32 vr_linux_socket_udp_init(VR_Linux_Socket_Udp* self, VR_Network_Ip_Addr addr);

        void vr_linux_socket_udp_uninit(VR_Linux_Socket_Udp* self);

        bool32 vr_linux_socket_udp_bind(VR_Linux_Socket_Udp* self);

        intptr vr_linux_socket_udp_write(VR_Linux_Socket_Udp* self, uint8* pntr, intptr size, VR_Network_Ip_Addr addr);

        intptr vr_linux_socket_udp_read(VR_Linux_Socket_Udp* self, uint8* pntr, intptr size, VR_Network_Ip_Addr* addr);

        VR_Network_Ip_Addr vr_linux_socket_udp_addr(VR_Linux_Socket_Udp* self);

    #endif

#endif

#endif
