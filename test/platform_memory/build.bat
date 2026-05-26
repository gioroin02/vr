@echo off

setlocal

set "comp_name=zig cc"
set "comp_version=99"
set "comp_include=-Isource"
set "comp_linking="
set "comp_warning=-Wall -Wpedantic -Wextra -Werror -Wfloat-conversion -Wimplicit-fallthrough -Wno-format -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function"
set "comp_flag=-O0 -g"

set "comp=%comp_name% -std=c%comp_version% %comp_flag% %comp_linking% %comp_include% %comp_warning%"

set "impl=%impl% source\vr_base_platform.c"
set "impl=%impl% source\vr_base_memory.c"
set "impl=%impl% source\vr_platform_memory.c"

set "test_memory=test\platform_memory\memory.c"

%comp% -o output\platform_memory.exe %impl% %test_memory%

endlocal
