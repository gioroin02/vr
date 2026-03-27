@echo off

set "compiler=zig cc --std=c99 --debug"

set "impl=%impl% src\vr_base\export.c"
set "impl=%impl% src\vr_base_memory\export.c"

set "test_read_write=test\base_memory\read_write.c"
set "test_arena=test\base_memory\arena.c"

%compiler% %impl% %test_read_write% -o base_memory_read_write.exe
%compiler% %impl% %test_arena%      -o base_memory_arena.exe
