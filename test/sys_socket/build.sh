compiler="gcc --std=c99 -Isource -Wall -g"

impl="$impl source/vr_base_platform.c"
impl="$impl source/vr_base_memory.c"
impl="$impl source/vr_sys_socket.c"

test_tcp_server="test/sys_socket/tcp_server.c"
test_tcp_client="test/sys_socket/tcp_client.c"
test_udp_server="test/sys_socket/udp_server.c"
test_udp_client="test/sys_socket/udp_client.c"

$compiler $impl $test_tcp_server -o sys_socket_tcp_server.exe
$compiler $impl $test_tcp_client -o sys_socket_tcp_client.exe
$compiler $impl $test_udp_server -o sys_socket_udp_server.exe
$compiler $impl $test_udp_client -o sys_socket_udp_client.exe
