comp_name="gcc"
comp_version="99"
comp_include="-Isource"
comp_linking=""
comp_warning="-Wall -Wpedantic -Wextra -Werror -Wfloat-conversion -Wimplicit-fallthrough -Wno-format -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function"
comp_flag="-O0 -g"

comp="$comp_name -std=c$comp_version $comp_flag $comp_linking $comp_include $comp_warning"

impl="$impl source/vr_base_platform.c"

test_type="test/base_platform/type.c"
test_macro="test/base_platform/macro.c"

$comp -o output/base_platform_type.out  $impl $test_type
$comp -o output/base_platform_macro.out $impl $test_macro
