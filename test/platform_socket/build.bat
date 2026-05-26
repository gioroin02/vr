@echo off

setlocal

set "comp_name=zig cc"
set "comp_version=99"
set "comp_linking=-lws2_32"
set "comp_include=-Isource"
set "comp_warning=-Wall -Wpedantic -Wextra -Werror -Wfloat-conversion -Wimplicit-fallthrough -Wno-format -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function"
set "comp_flag=-O0 -g"

set "comp=%comp_name% -std=c%comp_version% %comp_flag% %comp_linking% %comp_include% %comp_warning%"

set "impl=%impl% source\vr_base_platform.c"
set "impl=%impl% source\vr_base_memory.c"
set "impl=%impl% source\vr_platform_socket.c"

set "test_tcp_server=test\platform_socket\tcp_server.c"
set "test_tcp_client=test\platform_socket\tcp_client.c"
set "test_udp_server=test\platform_socket\udp_server.c"
set "test_udp_client=test\platform_socket\udp_client.c"

%comp% -o output\platform_socket_tcp_server.exe %impl% %test_tcp_server%
%comp% -o output\platform_socket_tcp_client.exe %impl% %test_tcp_client%
%comp% -o output\platform_socket_udp_server.exe %impl% %test_udp_server%
%comp% -o output\platform_socket_udp_client.exe %impl% %test_udp_client%

endlocal
