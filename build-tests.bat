@echo off
if not exist build mkdir build

set SOURCE_FILES=test\test-manips.cpp sym\symbol-table.o
set COMPILER_FLAGS=-Wall -fdiagnostics-absolute-paths
set IGNORED_WARNINGS=
set DEPENDENCIES=libgambatte.dll

echo on
clang++ -o build\test-manips.exe %COMPILER_FLAGS% %IGNORED_WARNINGS% %SOURCE_FILES% %DEPENDENCIES% && build\test-manips.exe