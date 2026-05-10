compiler="gcc --std=c99 -Isource -Wall -Wno-format -g"

impl="$impl source/vr_base_platform.c"
impl="$impl source/vr_base_memory.c"

test_utils="test/base_memory/utils.c"
test_arena="test/base_memory/arena.c"
test_pool="test/base_memory/pool.c"
test_stack="test/base_memory/stack.c"

$compiler $impl $test_utils -o base_memory_utils.exe
$compiler $impl $test_arena -o base_memory_arena.exe
$compiler $impl $test_pool  -o base_memory_pool.exe
$compiler $impl $test_stack -o base_memory_stack.exe
