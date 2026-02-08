@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x86
cd /d D:\src\cossacks-revamp-2017\tools
cl /nologo /W3 bmp_remap_test.c /Fe:bmp_remap_test.exe /link user32.lib
echo EXIT_CODE=%ERRORLEVEL%
