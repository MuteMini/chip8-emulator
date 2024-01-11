echo off
ninja -C build

IF %1%==test (.\build\test\unit_tests.exe)
IF %1%==src  (.\build\src\main.exe)