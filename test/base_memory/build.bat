@echo off

setlocal

set "compiler=zig cc --std=c99 -Wall -Wno-format -Isource -g"

set "impl=%impl% source\vr_base_platform.c"
set "impl=%impl% source\vr_base_memory.c"

set "test_utils=test\base_memory\utils.c"
set "test_arena=test\base_memory\arena.c"
set "test_pool=test\base_memory\pool.c"
set "test_stack=test\base_memory\stack.c"

%compiler% %impl% %test_utils% -o binary\base_memory_utils.exe
%compiler% %impl% %test_arena% -o binary\base_memory_arena.exe
%compiler% %impl% %test_pool%  -o binary\base_memory_pool.exe
%compiler% %impl% %test_stack% -o binary\base_memory_stack.exe

endlocal
