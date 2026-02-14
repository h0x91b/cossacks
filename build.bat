@echo off
setlocal
cd /d "%~dp0"

set MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
set SLN_OPTS=-p:PlatformToolset=v143 -p:WindowsTargetPlatformVersion=10.0.22621.0 -p:Configuration=Release -p:Platform=x86
set PROJ_OPTS=-p:PlatformToolset=v143 -p:WindowsTargetPlatformVersion=10.0.22621.0 -p:Configuration=Release -p:Platform=Win32

if not exist "src\Temp" mkdir "src\Temp"

REM Force build_info.cpp recompile so __DATE__/__TIME__ update every build
del /q "src\Temp\dmcr\build_info.obj" 2>nul

REM Fast path: if import libs already exist, do normal parallel build
if exist "src\Temp\dmcr.lib" if exist "src\Temp\IChat.lib" goto :incremental

echo === Bootstrap build (import libs missing) ===

REM Set up MSVC tools (cl.exe, lib.exe) for creating stub library
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x86 >nul 2>&1

echo [1/6] Building CommCore.lib...
%MSBUILD% "src\CommCore library\CommCore library.vcxproj" %PROJ_OPTS%
if errorlevel 1 goto :fail

echo [2/6] Creating stub IChat.lib...
echo void __stub(void){} > "%TEMP%\__stub.c"
cl /nologo /c "%TEMP%\__stub.c" /Fo"%TEMP%\__stub.obj"
lib /nologo /out:"src\Temp\IChat.lib" /machine:x86 "%TEMP%\__stub.obj"
del "%TEMP%\__stub.c" "%TEMP%\__stub.obj" 2>nul

echo [3/6] Building dmcr.exe (bootstrap pass)...
%MSBUILD% "src\Main executable\Cossacks.vcxproj" %PROJ_OPTS% /p:ForceFileOutput=UndefinedSymbolOnly
if not exist "src\Temp\dmcr.lib" goto :fail

echo [4/6] Building IChat.dll...
%MSBUILD% "src\IChat library\IChat library.vcxproj" %PROJ_OPTS%
if errorlevel 1 goto :fail

echo [5/6] Rebuilding dmcr.exe (final pass)...
%MSBUILD% "src\Main executable\Cossacks.vcxproj" %PROJ_OPTS% -t:Rebuild
if errorlevel 1 goto :fail

echo [6/6] Building IntExplorer.dll...
%MSBUILD% "src\IntExplorer library\IntExplorer library.vcxproj" %PROJ_OPTS%
if errorlevel 1 goto :fail

goto :copy

:incremental
echo === Incremental build ===
%MSBUILD% "src\Cossacks.sln" %SLN_OPTS% -m
if errorlevel 1 goto :fail

:copy
copy /Y "src\Testing\dmcr.exe" "Cossacks Back to War v1.52 (2025)\dmcr.exe"
copy /Y "src\Testing\IChat.dll" "Cossacks Back to War v1.52 (2025)\IChat.dll"
copy /Y "src\Testing\IntExplorer.dll" "Cossacks Back to War v1.52 (2025)\IntExplorer.dll"

echo.
echo === Building tools ===
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x86 >nul 2>&1
cl /nologo /O2 /W4 /D_CRT_SECURE_NO_WARNINGS "tools\gsc_extract.c" /Fe:"tools\gsc_extract.exe" /Fo:"tools\gsc_extract.obj"
if errorlevel 1 goto :fail

echo.
echo BUILD OK
pause
exit /b 0

:fail
echo.
echo BUILD FAILED
pause
exit /b 1
