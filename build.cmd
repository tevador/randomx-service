@echo off

set BUILD_DIR=build

set MSBUILD="c:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\MSBuild\Current\Bin\MSBuild.exe"
if not exist %MSBUILD% set MSBUILD="c:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe"
if not exist %MSBUILD% set MSBUILD="c:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\MSBuild\Current\Bin\MSBuild.exe"
if not exist %MSBUILD% set MSBUILD="c:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\Current\Bin\MSBuild.exe"
if not exist %MSBUILD% set MSBUILD="c:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\MSBuild\Current\Bin\MSBuild.exe"  
if not exist %MSBUILD% set MSBUILD="C:\Program Files (x86)\MSBuild\14.0\Bin\MsBuild.exe"
if not exist %MSBUILD% goto msbuild_not_found

if not exist %BUILD_DIR% mkdir %BUILD_DIR%

cd %BUILD_DIR%

cmake ..
if errorlevel 1 goto end

%MSBUILD% /nologo /fl /restore /p:Configuration=Release randomx-service.sln
if errorlevel 1 goto end

echo.
Release\randomx-service.exe -help
goto end

:msbuild_not_found
echo ERROR: MSBuild not found

:end
pause