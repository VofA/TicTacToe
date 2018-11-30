
@echo off

del app.exe

g++ main.cpp --std=c++17 -o app.exe

app.exe

pause