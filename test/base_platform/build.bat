@echo off

setlocal

set "compiler=gcc --std=c99 -Isource -Wall -Werror -g"

set "impl=%impl% source\vr_base_platform.c"

set "test_type=test\base_platform\type.c"
set "test_macro=test\base_platform\macro.c"

%compiler% %impl% %test_type%  -o base_platform_type.exe
%compiler% %impl% %test_macro% -o base_platform_macro.exe

endlocal
