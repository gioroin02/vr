@echo off

set "compiler=zig cc --std=c99 --debug"

set "impl=%impl% src\vr_base\export.c"

set "test_type=test\base\type.c"
set "test_define=test\base\define.c"

%compiler% %impl% %test_type%   -o base_type.exe
%compiler% %impl% %test_define% -o base_define.exe
