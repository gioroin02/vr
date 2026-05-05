compiler="gcc --std=c99 -Isource -Wall -g"

impl="$impl source/vr_base_platform.c"
impl="$impl source/vr_base_memory.c"
impl="$impl source/vr_sys_memory.c"

test_memory="test/sys_memory/memory.c"

$compiler $impl $test_memory -o sys_memory_memory.exe
