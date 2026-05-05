@echo off

setlocal

set "compiler=zig cc --std=c99 -Isource -Wall -g"

set "impl=%impl% source\vr_base_platform.c"
set "impl=%impl% source\vr_base_memory.c"
set "impl=%impl% source\vr_sys_memory.c"

set "test_memory=test\sys_memory\memory.c"

%compiler% %impl% %test_memory% -o sys_memory_memory.exe

endlocal
