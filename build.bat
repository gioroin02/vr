@echo off

cls

call .\test\base_platform\build.bat
call .\test\base_memory\build.bat

call .\test\platform_memory\build.bat
call .\test\platform_socket\build.bat

call .\example\build.bat
