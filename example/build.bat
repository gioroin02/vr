@echo off

setlocal

set "comp_name=zig cc"
set "comp_version=99"
set "comp_include=-Isource"
set "comp_linking=-lws2_32"
set "comp_warning=-Wall -Wpedantic -Wextra -Werror -Wfloat-conversion -Wimplicit-fallthrough -Wno-format -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function"
set "comp_flag=-O0 -g"

set "comp=%comp_name% -std=c%comp_version% %comp_flag% %comp_linking% %comp_include% %comp_warning%"

set "impl=%impl% source\vr_base_platform.c"
set "impl=%impl% source\vr_base_memory.c"
set "impl=%impl% source\vr_platform_memory.c"
set "impl=%impl% source\vr_platform_socket.c"

set "example_tcp_server_echo=example\tcp_server_echo.c"
set "example_tcp_client_echo=example\tcp_client_echo.c"
set "example_tcp_server_add=example\tcp_server_add.c"
set "example_tcp_client_add=example\tcp_client_add.c"

set "example_tcp_server_file=example\tcp_server_file.c"
set "example_tcp_client_file=example\tcp_client_file.c"

set "example_udp_server_echo=example\udp_server_echo.c"
set "example_udp_client_echo=example\udp_client_echo.c"
set "example_udp_server_add=example\udp_server_add.c"
set "example_udp_client_add=example\udp_client_add.c"

%comp% -o output\tcp_server_echo.exe %impl% %example_tcp_server_echo%
%comp% -o output\tcp_client_echo.exe %impl% %example_tcp_client_echo%
%comp% -o output\tcp_server_add.exe  %impl% %example_tcp_server_add%
%comp% -o output\tcp_client_add.exe  %impl% %example_tcp_client_add%

@rem %comp% -o output\tcp_server_file.exe %impl% %example_tcp_server_file%
@rem %comp% -o output\tcp_client_file.exe %impl% %example_tcp_client_file%

%comp% -o output\udp_server_echo.exe %impl% %example_udp_server_echo%
%comp% -o output\udp_client_echo.exe %impl% %example_udp_client_echo%
%comp% -o output\udp_server_add.exe  %impl% %example_udp_server_add%
%comp% -o output\udp_client_add.exe  %impl% %example_udp_client_add%

endlocal
