@echo off
chcp 65001 >nul
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 exit /b %errorlevel%
cl /nologo /Zi /W4 /O2 /MT /D_CRT_SECURE_NO_WARNINGS /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\um" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\shared" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\ucrt" "C:\Users\Пользователь\Desktop\VKACTF2026\pwn\win_pathflip\src\chall\chall.c" /Fe"C:\Users\Пользователь\Desktop\VKACTF2026\pwn\win_pathflip\build\x64\Release\chall.exe" /link /libpath:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\um\x64" /libpath:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\ucrt\x64"
if errorlevel 1 exit /b %errorlevel%