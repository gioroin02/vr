@echo off

setlocal

set "compiler=zig cc --std=c99 -Isource -Wall -g -lws2_32"

set "impl=%impl% source\vr_base_platform.c"
set "impl=%impl% source\vr_base_memory.c"
set "impl=%impl% source\vr_sys_memory.c"
set "impl=%impl% source\vr_sys_socket.c"

set "example_tcp_server_echo=example\tcp_server_echo.c"
set "example_tcp_client_echo=example\tcp_client_echo.c"
set "example_tcp_server_add=example\tcp_server_add.c"
set "example_tcp_client_add=example\tcp_client_add.c"

%compiler% %impl% %example_tcp_server_echo% -o tcp_server_echo.exe
%compiler% %impl% %example_tcp_client_echo% -o tcp_client_echo.exe
%compiler% %impl% %example_tcp_server_add%  -o tcp_server_add.exe
%compiler% %impl% %example_tcp_client_add%  -o tcp_client_add.exe

endlocal
