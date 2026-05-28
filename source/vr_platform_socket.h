#ifndef VR_PLATFORM_SOCKET_H
#define VR_PLATFORM_SOCKET_H

#include "vr_base_memory.h"

typedef enum VrAddressIpKind
{
    VrAddressIpKind_None,
    VrAddressIpKind_V4,
    VrAddressIpKind_V6,
}
VrAddressIpKind;

#define VR_ADDRESS_IPV4_SIZE  ((VrSint) 4)
#define VR_ADDRESS_IPV4_EMPTY ((VrAddressIpV4) {0})

#define VR_ADDRESS_IPV4_LOCAL \
    ((VrAddressIpV4) {.elements.members = {.elem_0 = 0x7F, .elem_3 = 0x01}})

typedef struct VrAddressIpV4
{
    union
    {
        VrUint8 array[VR_ADDRESS_IPV4_SIZE];

        struct
        {
            VrUint8 elem_0, elem_1, elem_2, elem_3;
        }
        members;
    }
    elements;
}
VrAddressIpV4;

#define VR_ADDRESS_IPV6_SIZE  ((VrSint) 16)
#define VR_ADDRESS_IPV6_EMPTY ((VrAddressIpV6) {0})

#define VR_ADDRESS_IPV6_LOCAL \
    ((VrAddressIpV6) {.elements.members = {.elem_15 = 0x01}})

typedef struct VrAddressIpV6
{
    union
    {
        VrUint8 array[VR_ADDRESS_IPV6_SIZE];

        struct
        {
            VrUint8 elem_0,  elem_1,  elem_2,  elem_3;
            VrUint8 elem_4,  elem_5,  elem_6,  elem_7;
            VrUint8 elem_8,  elem_9,  elem_10, elem_11;
            VrUint8 elem_12, elem_13, elem_14, elem_15;
        }
        members;
    }
    elements;
}
VrAddressIpV6;

typedef struct VrAddressIp
{
    VrAddressIpKind kind;

    union
    {
        VrAddressIpV4 v4;
        VrAddressIpV6 v6;
    }
    ip;

    VrUint16 port;
}
VrAddressIp;

VrAddressIp vr_address_ip_none(void);

#define vr_address_ipv4_empty() \
    vr_address_ip_empty(VrAddressIpKind_V4)

#define vr_address_ipv6_empty() \
    vr_address_ip_empty(VrAddressIpKind_V6)

VrAddressIp vr_address_ip_empty(VrAddressIpKind kind);

#define vr_address_ipv4_local(port) \
    vr_address_ip_local(VrAddressIpKind_V4, (port))

#define vr_address_ipv6_local(port) \
    vr_address_ip_local(VrAddressIpKind_V6, (port))

VrAddressIp vr_address_ip_local(VrAddressIpKind kind, VrUint16 port);

VrAddressIp vr_address_ipv4(VrAddressIpV4 ip4, VrUint16 port);

VrAddressIp vr_address_ipv6(VrAddressIpV6 ip6, VrUint16 port);

VrBool32 vr_address_ip_is_equal(VrAddressIp self, VrAddressIp other);

typedef struct VrTcpListener
{
    void* impl;
}
VrTcpListener;

VrTcpListener vr_tcp_listener_reserve(VrAlloc* alloc);

VrBool32 vr_tcp_listener_init(VrTcpListener self, VrAddressIp addr);

void vr_tcp_listener_uninit(VrTcpListener self);

VrBool32 vr_tcp_listener_bind_and_listen(VrTcpListener self);

VrAddressIp vr_tcp_listener_addr(VrTcpListener self);

typedef struct VrTcpSocket
{
    void* impl;
}
VrTcpSocket;

VrTcpSocket vr_tcp_socket_reserve(VrAlloc* alloc);

VrBool32 vr_tcp_socket_init(VrTcpSocket self, VrAddressIp addr);

void vr_tcp_socket_uninit(VrTcpSocket self);

VrBool32 vr_tcp_socket_bind(VrTcpSocket self);

VrBool32 vr_tcp_socket_accept(VrTcpSocket self, VrTcpListener listener);

VrBool32 vr_tcp_socket_connect(VrTcpSocket self, VrAddressIp addr);

VrSint vr_tcp_socket_write(VrTcpSocket self, VrUint8* pntr, VrSint size);

VrSint vr_tcp_socket_write_all(VrTcpSocket self, VrUint8* pntr, VrSint size);

VrSint vr_tcp_socket_read(VrTcpSocket self, VrUint8* pntr, VrSint size);

VrAddressIp vr_tcp_socket_addr(VrTcpSocket self);

typedef struct VrUdpSocket
{
    void* impl;
}
VrUdpSocket;

VrUdpSocket vr_udp_socket_reserve(VrAlloc* alloc);

VrBool32 vr_udp_socket_init(VrUdpSocket self, VrAddressIp addr);

void vr_udp_socket_uninit(VrUdpSocket self);

VrBool32 vr_udp_socket_bind(VrUdpSocket self);

VrSint vr_udp_socket_write(VrUdpSocket self, VrUint8* pntr, VrSint size, VrAddressIp addr);

VrSint vr_udp_socket_write_all(VrUdpSocket self, VrUint8* pntr, VrSint size, VrAddressIp addr);

VrSint vr_udp_socket_read(VrUdpSocket self, VrUint8* pntr, VrSint size, VrAddressIp* addr);

VrAddressIp vr_udp_socket_addr(VrUdpSocket self);

#endif

#if defined VR_PLATFORM_SOCKET_PRIVATE

    #if VR_SYSTEM == VR_SYSTEM_WINDOWS

        #define WIN32_LEAN_AND_MEAN
        #define NOGDI
        #include <windows.h>

        #include <winsock2.h>
        #include <ws2tcpip.h>
        #include <mswsock.h>

        typedef struct sockaddr_storage sockaddr_storage_t;
        typedef struct sockaddr         sockaddr_t;
        typedef struct sockaddr_in      sockaddr_ipv4_t;
        typedef struct sockaddr_in6     sockaddr_ipv6_t;

        VrBool32 vr_win32_socket_start(void);

        void vr_win32_socket_stop(void);

        sockaddr_storage_t vr_win32_sockaddr_make(VrAddressIp addr);

        sockaddr_storage_t vr_win32_sockaddr_make_any(VrAddressIpKind kind, VrUint16 port);

        VrSint vr_win32_sockaddr_size(sockaddr_storage_t* self);

        VrAddressIp vr_win32_sockaddr_addr(sockaddr_storage_t* self);

        typedef struct VrWin32TcpListener
        {
            SOCKET handle;
            void*  queue;

            sockaddr_storage_t address;
        }
        VrWin32TcpListener;

        VrWin32TcpListener* vr_win32_tcp_listener_reserve(VrAlloc* alloc);

        VrBool32 vr_win32_tcp_listener_init(VrWin32TcpListener* self, VrAddressIp addr);

        void vr_win32_tcp_listener_uninit(VrWin32TcpListener* self);

        VrBool32 vr_win32_tcp_listener_bind(VrWin32TcpListener* self);

        VrBool32 vr_win32_tcp_listener_listen(VrWin32TcpListener* listener);

        VrAddressIp vr_win32_tcp_listener_addr(VrWin32TcpListener* self);

        typedef struct VrWin32TcpSocket
        {
            SOCKET handle;
            void*  queue;

            sockaddr_storage_t address;
        }
        VrWin32TcpSocket;

        VrWin32TcpSocket* vr_win32_tcp_socket_reserve(VrAlloc* alloc);

        VrBool32 vr_win32_tcp_socket_init(VrWin32TcpSocket* self, VrAddressIp addr);

        void vr_win32_tcp_socket_uninit(VrWin32TcpSocket* self);

        VrBool32 vr_win32_tcp_socket_bind(VrWin32TcpSocket* self);

        VrBool32 vr_win32_tcp_socket_accept(VrWin32TcpSocket* self, VrWin32TcpListener* listener);

        VrBool32 vr_win32_tcp_socket_connect(VrWin32TcpSocket* self, VrAddressIp addr);

        VrSint vr_win32_tcp_socket_write(VrWin32TcpSocket* self, VrUint8* pntr, VrSint size);

        VrSint vr_win32_tcp_socket_read(VrWin32TcpSocket* self, VrUint8* pntr, VrSint size);

        VrAddressIp vr_win32_tcp_socket_addr(VrWin32TcpSocket* self);

        typedef struct VrWin32UdpSocket
        {
            SOCKET handle;
            void*  queue;

            sockaddr_storage_t address;
        }
        VrWin32UdpSocket;

        VrWin32UdpSocket* vr_win32_udp_socket_reserve(VrAlloc* alloc);

        VrBool32 vr_win32_udp_socket_init(VrWin32UdpSocket* self, VrAddressIp addr);

        void vr_win32_udp_socket_uninit(VrWin32UdpSocket* self);

        VrBool32 vr_win32_udp_socket_bind(VrWin32UdpSocket* self);

        VrSint vr_win32_udp_socket_write(VrWin32UdpSocket* self, VrUint8* pntr, VrSint size, VrAddressIp addr);

        VrSint vr_win32_udp_socket_read(VrWin32UdpSocket* self, VrUint8* pntr, VrSint size, VrAddressIp* addr);

        VrAddressIp vr_win32_udp_socket_addr(VrWin32UdpSocket* self);

    #elif VR_SYSTEM == VR_SYSTEM_LINUX

        #include <unistd.h>
        #include <errno.h>

        #include <sys/socket.h>
        #include <netinet/in.h>

        typedef struct sockaddr_storage sockaddr_storage_t;
        typedef struct sockaddr         sockaddr_t;
        typedef struct sockaddr_in      sockaddr_ipv4_t;
        typedef struct sockaddr_in6     sockaddr_ipv6_t;

        sockaddr_storage_t vr_linux_sockaddr_make(VrAddressIp addr);

        sockaddr_storage_t vr_linux_sockaddr_make_any(VrAddressIpKind kind, VrUint16 port);

        VrSint vr_linux_sockaddr_size(sockaddr_storage_t* self);

        VrAddressIp vr_linux_sockaddr_addr(sockaddr_storage_t* self);

        typedef struct VrLinuxcpListener
        {
            int   handle;
            void* queue;

            sockaddr_storage_t address;
        }
        VrLinuxTcpListener;

        VrLinuxTcpListener* vr_linux_tcp_listener_reserve(VrAlloc* alloc);

        VrBool32 vr_linux_tcp_listener_init(VrLinuxTcpListener* self, VrAddressIp addr);

        void vr_linux_tcp_listener_uninit(VrLinuxTcpListener* self);

        VrBool32 vr_linux_tcp_listener_bind(VrLinuxTcpListener* self);

        VrBool32 vr_linux_tcp_listener_listen(VrLinuxTcpListener* self);

        VrAddressIp vr_linux_tcp_listener_addr(VrLinuxTcpListener* self);

        typedef struct VrLinuxTcpSocket
        {
            int   handle;
            void* queue;

            sockaddr_storage_t address;
        }
        VrLinuxTcpSocket;

        VrLinuxTcpSocket* vr_linux_tcp_socket_reserve(VrAlloc* alloc);

        VrBool32 vr_linux_tcp_socket_init(VrLinuxTcpSocket* self, VrAddressIp addr);

        void vr_linux_tcp_socket_uninit(VrLinuxTcpSocket* self);

        VrBool32 vr_linux_tcp_socket_bind(VrLinuxTcpSocket* self);

        VrBool32 vr_linux_tcp_socket_accept(VrLinuxTcpSocket* self, VrLinuxTcpListener* listener);

        VrBool32 vr_linux_tcp_socket_connect(VrLinuxTcpSocket* self, VrAddressIp addr);

        VrSint vr_linux_tcp_socket_write(VrLinuxTcpSocket* self, VrUint8* pntr, VrSint size);

        VrSint vr_linux_tcp_socket_read(VrLinuxTcpSocket* self, VrUint8* pntr, VrSint size);

        VrAddressIp vr_linux_tcp_socket_addr(VrLinuxTcpSocket* self);

        typedef struct VrLinuxUdpSocket
        {
            int   handle;
            void* queue;

            sockaddr_storage_t address;
        }
        VrLinuxUdpSocket;

        VrLinuxUdpSocket* vr_linux_udp_socket_reserve(VrAlloc* alloc);

        VrBool32 vr_linux_udp_socket_init(VrLinuxUdpSocket* self, VrAddressIp addr);

        void vr_linux_udp_socket_uninit(VrLinuxUdpSocket* self);

        VrBool32 vr_linux_udp_socket_bind(VrLinuxUdpSocket* self);

        VrSint vr_linux_udp_socket_write(VrLinuxUdpSocket* self, VrUint8* pntr, VrSint size, VrAddressIp addr);

        VrSint vr_linux_udp_socket_read(VrLinuxUdpSocket* self, VrUint8* pntr, VrSint size, VrAddressIp* addr);

        VrAddressIp vr_linux_udp_socket_addr(VrLinuxUdpSocket* self);

    #endif

#endif
