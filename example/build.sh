comp_name="zig cc"
comp_version="99"
comp_include="-Isource"
comp_linking="-lws2_32"
comp_warning="-Wall -Wextra -Wpedantic -Wfloat-conversion -Wimplicit-fallthrough -Wno-format -Wno-unused-parameter -Werror"
comp_flag="-O0 -g"

comp="$comp_name -std=ccomp_version $comp_flag $comp_linking $comp_include $comp_warning"

impl="$impl source/vr_base_platform.c"
impl="$impl source/vr_base_memory.c"
impl="$impl source/vr_system_memory.c"
impl="$impl source/vr_system_socket.c"

example_tcp_server_echo="example/tcp_server_echo.c"
example_tcp_client_echo="example/tcp_client_echo.c"
example_tcp_server_add="example/tcp_server_add.c"
example_tcp_client_add="example/tcp_client_add.c"

example_tcp_server_file="example/tcp_server_file.c"
example_tcp_client_file="example/tcp_client_file.c"

example_udp_server_echo="example/udp_server_echo.c"
example_udp_client_echo="example/udp_client_echo.c"
example_udp_server_add="example/udp_server_add.c"
example_udp_client_add="example/udp_client_add.c"

$comp -o output/tcp_server_echo.exe $impl $example_tcp_server_echo
$comp -o output/tcp_client_echo.exe $impl $example_tcp_client_echo
$comp -o output/tcp_server_add.exe  $impl $example_tcp_server_add
$comp -o output/tcp_client_add.exe  $impl $example_tcp_client_add

$comp -o output/tcp_server_file.exe $impl $example_tcp_server_file
$comp -o output/tcp_client_file.exe $impl $example_tcp_client_file

$comp -o output/udp_server_echo.exe $impl $example_udp_server_echo
$comp -o output/udp_client_echo.exe $impl $example_udp_client_echo
$comp -o output/udp_server_add.exe  $impl $example_udp_server_add
$comp -o output/udp_client_add.exe  $impl $example_udp_client_add
