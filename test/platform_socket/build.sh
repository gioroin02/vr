comp_name="gcc"
comp_version="99"
comp_include="-Isource"
comp_linking=""
comp_warning="-Wall -Wpedantic -Wextra -Werror -Wfloat-conversion -Wimplicit-fallthrough -Wno-format -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function"
comp_flag="-O0 -g"

comp="$comp_name -std=c$comp_version $comp_flag $comp_linking $comp_include $comp_warning"

impl="$impl source/vr_base_platform.c"
impl="$impl source/vr_base_memory.c"
impl="$impl source/vr_platform_socket.c"

test_tcp_server="test/platform_socket/tcp_server.c"
test_tcp_client="test/platform_socket/tcp_client.c"
test_udp_server="test/platform_socket/udp_server.c"
test_udp_client="test/platform_socket/udp_client.c"

$comp -o output/platform_socket_tcp_server.out $impl $test_tcp_server
$comp -o output/platform_socket_tcp_client.out $impl $test_tcp_client
$comp -o output/platform_socket_udp_server.out $impl $test_udp_server
$comp -o output/platform_socket_udp_client.out $impl $test_udp_client
