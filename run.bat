@echo off
g++ main.cpp -o main 

if %errorlevel% neq 0 (
    echo Compilation failed.
    pause
    exit /b %errorlevel%
)
echo Compilation successful. Running program...
main.exe
pause
