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

set "test_utils=test\base_memory\utils.c"
set "test_arena=test\base_memory\arena.c"
set "test_pool=test\base_memory\pool.c"
set "test_stack=test\base_memory\stack.c"

%comp% -o output\base_memory_utils.exe %impl% %test_utils%
%comp% -o output\base_memory_arena.exe %impl% %test_arena%
%comp% -o output\base_memory_pool.exe  %impl% %test_pool%
%comp% -o output\base_memory_stack.exe %impl% %test_stack%

endlocal
