compiler="gcc --std=c99 -Isource -Wall -Wno-format -g"

impl="$impl source/vr_base_platform.c"
impl="$impl source/vr_base_memory.c"
impl="$impl source/vr_system_memory.c"

test_memory="test/system_memory/memory.c"

$compiler $impl $test_memory -o system_memory_memory.exe
