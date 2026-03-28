@echo off

set "compiler=zig cc --std=c99 -lws2_32"

set "impl=%impl% src\vr_base\export.c"
set "impl=%impl% src\vr_base_memory\export.c"
set "impl=%impl% src\vr_sys_memory\export.c"
set "impl=%impl% src\vr_sys_socket\export.c"

set "example_tcp_server_first=example\tcp_server_first.c"
set "example_tcp_client_first=example\tcp_client_first.c"
set "example_tcp_server_add=example\tcp_server_add.c"
set "example_tcp_client_add=example\tcp_client_add.c"

%compiler% %impl% %example_tcp_server_first% -o tcp_server_first.exe
%compiler% %impl% %example_tcp_client_first% -o tcp_client_first.exe
%compiler% %impl% %example_tcp_server_add%   -o tcp_server_add.exe
%compiler% %impl% %example_tcp_client_add%   -o tcp_client_add.exe
