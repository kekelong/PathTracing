@echo off
cd /d %~dp0
cmake -S . -B build
cmake --build build --config Release
pause