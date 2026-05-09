@echo off

setlocal

set "compiler=gcc --std=c99 -Isource -Wall -g"

set "impl=%impl% source\vr_base_platform.c"
set "impl=%impl% source\vr_base_memory.c"
set "impl=%impl% source\vr_system_memory.c"

set "test_memory=test\system_memory\memory.c"

%compiler% %impl% %test_memory% -o system_memory_memory.exe

endlocal
