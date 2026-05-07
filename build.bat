@echo off

cls

call .\test\base_platform\build.bat
call .\test\base_memory\build.bat

call .\test\system_memory\build.bat
call .\test\system_socket\build.bat

call .\example\build.bat
