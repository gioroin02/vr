#ifndef VR_SYS_SOCKET_LINUX_UTILS_H
#define VR_SYS_SOCKET_LINUX_UTILS_H

#include "import.h"

sockaddr_storage_t vr_linux_sockaddr_make(VR_Endpoint_IP endpoint);

sockaddr_storage_t vr_linux_sockaddr_make_any(VR_Endpoint_IP_Kind kind, uint16 port);

intptr vr_linux_sockaddr_get_size(sockaddr_storage_t* self);

VR_Endpoint_IP vr_linux_sockaddr_get_endpoint(sockaddr_storage_t* self);

#endif
