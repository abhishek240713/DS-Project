@echo off
setlocal
cd /d "%~dp0"

echo Building Library Management System...
gcc -std=c99 -Wall -Wextra -Wpedantic -O2 main.c books.c members.c transactions.c reservations.c stack.c queue.c bst.c sorting.c search.c file_manager.c reports.c utils.c -o library.exe
if errorlevel 1 (
    echo.
    echo Build failed. Make sure GCC/MinGW is installed and available in PATH.
    exit /b 1
)
echo.
echo Build successful: library.exe
endlocal
