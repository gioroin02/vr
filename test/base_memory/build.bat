@echo off

setlocal

set "compiler=gcc --std=c99 -Isource -Wall -g"

set "impl=%impl% source\vr_base_platform.c"
set "impl=%impl% source\vr_base_memory.c"

set "test_utils=test\base_memory\utils.c"
set "test_arena=test\base_memory\arena.c"
set "test_pool=test\base_memory\pool.c"
set "test_stack=test\base_memory\stack.c"

%compiler% %impl% %test_utils% -o base_memory_utils.exe
%compiler% %impl% %test_arena% -o base_memory_arena.exe
%compiler% %impl% %test_pool%  -o base_memory_pool.exe
%compiler% %impl% %test_stack% -o base_memory_stack.exe

endlocal
