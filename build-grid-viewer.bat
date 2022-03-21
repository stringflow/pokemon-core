@echo off
if not exist build mkdir build

set SOURCE_FILES=test\grid-viewer.cpp sym\symbol-table-msvc.o
set COMPILER_FLAGS=-DCOMPILER_MSVC -target x86_64-pc-windows-msvc -g -gcodeview -Wall -fdiagnostics-absolute-paths
set IGNORED_WARNINGS=-Wno-deprecated-declarations -Wno-unused-variable
set DEPENDENCIES=libgambatte.lib -luser32 -lgdi32

echo on
clang++ -o build\grid-viewer.exe %COMPILER_FLAGS% %IGNORED_WARNINGS% %SOURCE_FILES% %DEPENDENCIES%