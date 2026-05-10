compiler="gcc --std=c99 -Isource -Wall -Wno-format -g"

impl="$impl source/vr_base_platform.c"
impl="$impl source/vr_base_memory.c"
impl="$impl source/vr_system_memory.c"
impl="$impl source/vr_system_socket.c"

example_tcp_server_echo="example/tcp_server_echo.c"
example_tcp_client_echo="example/tcp_client_echo.c"
example_tcp_server_add="example/tcp_server_add.c"
example_tcp_client_add="example/tcp_client_add.c"

$compiler $impl $example_tcp_server_echo -o tcp_server_echo.exe
$compiler $impl $example_tcp_client_echo -o tcp_client_echo.exe
$compiler $impl $example_tcp_server_add  -o tcp_server_add.exe
$compiler $impl $example_tcp_client_add  -o tcp_client_add.exe
