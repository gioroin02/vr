@echo off

setlocal

set "compiler=gcc --std=c99 -Isource -Wall -Werror -g"
set "linker=-lws2_32"

set "impl=%impl% source\vr_base_platform.c"
set "impl=%impl% source\vr_base_memory.c"
set "impl=%impl% source\vr_system_socket.c"

set "test_tcp_server=test\system_socket\tcp_server.c"
set "test_tcp_client=test\system_socket\tcp_client.c"
set "test_udp_server=test\system_socket\udp_server.c"
set "test_udp_client=test\system_socket\udp_client.c"

%compiler% %impl% %test_tcp_server% %linker% -o system_socket_tcp_server.exe
%compiler% %impl% %test_tcp_client% %linker% -o system_socket_tcp_client.exe
%compiler% %impl% %test_udp_server% %linker% -o system_socket_udp_server.exe
%compiler% %impl% %test_udp_client% %linker% -o system_socket_udp_client.exe

endlocal
