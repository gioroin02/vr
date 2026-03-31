#ifndef VR_SYS_SOCKET_LINUX_IMPORT_H
#define VR_SYS_SOCKET_LINUX_IMPORT_H

#include "../export.h"

#include <unistd.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>

typedef struct sockaddr_storage sockaddr_storage_t;
typedef struct sockaddr         sockaddr_t;
typedef struct sockaddr_in      sockaddr_ip4_t;
typedef struct sockaddr_in6     sockaddr_ip6_t;

#endif
