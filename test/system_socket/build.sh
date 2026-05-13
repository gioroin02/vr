compiler="gcc --std=c99 -Wall -Wno-format -Isource -g"

impl="$impl source/vr_base_platform.c"
impl="$impl source/vr_base_memory.c"
impl="$impl source/vr_system_socket.c"

test_tcp_server="test/system_socket/tcp_server.c"
test_tcp_client="test/system_socket/tcp_client.c"
test_udp_server="test/system_socket/udp_server.c"
test_udp_client="test/system_socket/udp_client.c"

$compiler $impl $test_tcp_server -o temp/system_socket_tcp_server.out
$compiler $impl $test_tcp_client -o temp/system_socket_tcp_client.out
$compiler $impl $test_udp_server -o temp/system_socket_udp_server.out
$compiler $impl $test_udp_client -o temp/system_socket_udp_client.out
