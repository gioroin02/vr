compiler="gcc --std=c99 -Wall -Wno-format -Isource -g"

impl="$impl source/vr_base_platform.c"
impl="$impl source/vr_base_memory.c"

test_utils="test/base_memory/utils.c"
test_arena="test/base_memory/arena.c"
test_pool="test/base_memory/pool.c"
test_stack="test/base_memory/stack.c"

$compiler $impl $test_utils -o binary/base_memory_utils.out
$compiler $impl $test_arena -o binary/base_memory_arena.out
$compiler $impl $test_pool  -o binary/base_memory_pool.out
$compiler $impl $test_stack -o binary/base_memory_stack.out
