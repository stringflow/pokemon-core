@echo off
if not exist build mkdir build

set SOURCE_FILES=src\gameboy.cpp sym\symbol-table.o
set COMPILER_FLAGS=-Wall -Wextra -O2 -shared -fdiagnostics-absolute-paths
set IGNORED_WARNINGS=-Wno-missing-field-initializers
set DEPENDENCIES=libgambatte.dll

echo on
clang++ -o build\pokemon-core.dll %COMPILER_FLAGS% %IGNORED_WARNINGS% %SOURCE_FILES% %DEPENDENCIES%