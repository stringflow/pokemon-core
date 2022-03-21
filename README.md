# pokemon-core

pokemon-core is a work-in-progress C DLL that interfaces [gambatte-speedrun's shared library](https://github.com/pokemon-speedrunning/gambatte-core) and provides commonly done functionality for use in scripting, botting, or TASing. Any function marked with `DLLEXPORT` can be called from any other language that calling C libraries.

# Code Layout

All DLL code is located in the `src` directory, with all code being `#inlcude` into the main translation unit. There is a pre-compiled symbol table located in the `sym` directory that will be linked against, too. Embedding the sym table into the DLL instead of parsing it from an external file causes a speedup of the code, while making the DLL several megabytes big.  
Testing code lives in the `test` directory, it currently contains code to generate the symbol table, manip tests, and a program to debug the grid overlay.

# Build Instructions

Currently only build scripts for Windows exist since that is the platform that I am using to develop this project.  

Install clang and run `build.bat` to build the DLL.  
Run `build-tests.bat` to build the manip testing executable.  
Run `build-grid-viewer.bat` to build the grid viewer.  
You will have to put ROM files into the respective directory within `test` to run the executables. (The executables expect to be ran from the root directory of the project)