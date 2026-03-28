@echo off

call .\test\base\build.bat
call .\test\base_memory\build.bat
call .\test\sys_memory\build.bat
call .\test\sys_socket\build.bat

call .\example\build.bat
