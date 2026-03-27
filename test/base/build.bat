@echo off

set "compiler=zig cc --std=c99 --debug"

set "impl=%impl% src\vr_base\export.c"

set "test_macros=test\base\macros.c"
set "test_types=test\base\types.c"

%compiler% %impl% %test_macros% -o base_macros.exe
%compiler% %impl% %test_types%  -o base_types.exe
