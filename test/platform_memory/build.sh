comp_name="gcc"
comp_version="99"
comp_include="-Isource"
comp_linking=""
comp_warning="-Wall -Wpedantic -Wextra -Werror -Wfloat-conversion -Wimplicit-fallthrough -Wno-format -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function"
comp_flag="-O0 -g"

comp="$comp_name -std=c$comp_version $comp_flag $comp_linking $comp_include $comp_warning"

impl="$impl source/vr_base_platform.c"
impl="$impl source/vr_base_memory.c"
impl="$impl source/vr_platform_memory.c"

test_memory="test/platform_memory/memory.c"

$comp -o output/platform_memory.out $impl $test_memory
