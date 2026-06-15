@echo off
chcp 65001 >nul
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 exit /b %errorlevel%
cl /nologo /c /Zi /W4 /WX- /Gz /kernel /GS- /GR- /D_AMD64_=1 /DWIN64=1 /D_WIN64=1 /D_KERNEL_MODE=1 /D_NTDRIVER_=1 /DWINNT=1 /D_WIN32_WINNT=0x0A00 /DNTDDI_VERSION=0x0A000008 /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\km" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\shared" /Fo"C:\Users\Пользователь\Desktop\VKACTF2026\pwn\win_pathflip\build\x64\Release\obj\VStorCache.obj" "C:\Users\Пользователь\Desktop\VKACTF2026\pwn\win_pathflip\src\driver\VStorCache.c"
if errorlevel 1 exit /b %errorlevel%
link /nologo /debug /driver /subsystem:native /entry:DriverEntry /nodefaultlib /out:"C:\Users\Пользователь\Desktop\VKACTF2026\pwn\win_pathflip\build\x64\Release\VStorCache.sys" /libpath:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\km\x64" "C:\Users\Пользователь\Desktop\VKACTF2026\pwn\win_pathflip\build\x64\Release\obj\VStorCache.obj" ntoskrnl.lib hal.lib BufferOverflowK.lib wdmsec.lib
if errorlevel 1 exit /b %errorlevel%