#ifndef VR_SYS_SOCKET_WIN32_IMPORT_H
#define VR_SYS_SOCKET_WIN32_IMPORT_H

#include "../export.h"

#define WIN32_LEAN_AND_MEAN
#define NOGDI

#pragma comment(lib, "ws2_32.lib")

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>

typedef struct sockaddr_storage sockaddr_storage_t;
typedef struct sockaddr         sockaddr_t;
typedef struct sockaddr_in      sockaddr_ip4_t;
typedef struct sockaddr_in6     sockaddr_ip6_t;

#endif
