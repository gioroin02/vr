@echo off

setlocal

set "compiler=gcc --std=c99 -Isource -Wall -Wno-format -g"
set "linker=-lws2_32"

set "impl=%impl% source\vr_base_platform.c"
set "impl=%impl% source\vr_base_memory.c"
set "impl=%impl% source\vr_system_memory.c"
set "impl=%impl% source\vr_system_socket.c"

set "example_tcp_server_echo=example\tcp_server_echo.c"
set "example_tcp_client_echo=example\tcp_client_echo.c"
set "example_tcp_server_add=example\tcp_server_add.c"
set "example_tcp_client_add=example\tcp_client_add.c"

%compiler% %impl% %example_tcp_server_echo% %linker% -o tcp_server_echo.exe
%compiler% %impl% %example_tcp_client_echo% %linker% -o tcp_client_echo.exe
%compiler% %impl% %example_tcp_server_add%  %linker% -o tcp_server_add.exe
%compiler% %impl% %example_tcp_client_add%  %linker% -o tcp_client_add.exe

endlocal
