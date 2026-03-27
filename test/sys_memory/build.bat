@echo off

set "compiler=zig cc --std=c99"

set "impl=%impl% src\vr_base\export.c"
set "impl=%impl% src\vr_base_memory\export.c"
set "impl=%impl% src\vr_sys_memory\export.c"

set "test_memory=test\sys_memory\memory.c"

%compiler% %impl% %test_memory% -o sys_memory_memory.exe
