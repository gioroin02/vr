#ifndef VR_SYS_SOCKET_WIN32_UTILS_H
#define VR_SYS_SOCKET_WIN32_UTILS_H

#include "import.h"

extern LPFN_CONNECTEX WSAConnectEx;
extern LPFN_ACCEPTEX  WSAAcceptEx;

bool32 vr_win32_socket_start();

void vr_win32_socket_stop();

sockaddr_storage_t vr_win32_sockaddr_make(VR_Endpoint_IP endpoint);

sockaddr_storage_t vr_win32_sockaddr_make_any(VR_Endpoint_IP_Kind kind, uint16 port);

intptr vr_win32_sockaddr_get_size(sockaddr_storage_t* self);

VR_Endpoint_IP vr_win32_sockaddr_get_endpoint(sockaddr_storage_t* self);

#endif
