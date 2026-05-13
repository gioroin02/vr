compiler="gcc --std=c99 -Wall -Wno-format -Isource -g"

impl="$impl source/vr_base_platform.c"

test_type="test/base_platform/type.c"
test_macro="test/base_platform/macro.c"

$compiler $impl $test_type  -o temp/base_platform_type.out
$compiler $impl $test_macro -o temp/base_platform_macro.out
