@echo off
setlocal
cd /d "%~dp0"

if not exist library.exe (
    call build.bat
    if errorlevel 1 exit /b 1
)

"%~dp0library.exe"
endlocal
