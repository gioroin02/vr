compiler="gcc --std=c99 --debug"

impl="$impl src/vr_base/export.c"

test_type="test/base/type.c"
test_define="test/base/define.c"

$compiler $impl $test_type   -o base_type.exe
$compiler $impl $test_define -o base_define.exe
