# memory.cpp
## Table of Contents
- [gameboy_cpuread](#gameboy_cpuread)
- [gameboy_cpureadpacked](#gameboy_cpureadpacked)
- [gameboy_cpureadpackedsym](#gameboy_cpureadpackedsym)
- [gameboy_cpureadsym](#gameboy_cpureadsym)
- [gameboy_cpuwrite](#gameboy_cpuwrite)
- [gameboy_cpuwritepacked](#gameboy_cpuwritepacked)
- [gameboy_cpuwritepackedsym](#gameboy_cpuwritepackedsym)
- [gameboy_cpuwritesym](#gameboy_cpuwritesym)
- [gameboy_getregisters](#gameboy_getregisters)
- [gameboy_getromtitle](#gameboy_getromtitle)
- [gameboy_getstatesize](#gameboy_getstatesize)
- [gameboy_loadstate](#gameboy_loadstate)
- [gameboy_loadstatefromdisk](#gameboy_loadstatefromdisk)
- [gameboy_savestate](#gameboy_savestate)
- [gameboy_savestatetodisk](#gameboy_savestatetodisk)
- [gameboy_setregisters](#gameboy_setregisters)
## Functions
### gameboy_cpuread
`(GameBoy *gb, int address) -> u8`  
Reads a single byte from the CPU bus. This includes all RAM, ROM, MMIO, etc. as it is visible to the CPU (including mappers).  
There is no cycle cost to any of these external CPU reads.
### gameboy_cpureadpacked
`(GameBoy *gb, int address, void *dest, int size, bool big_endian) -> void`  
Reads multiple bytes from the CPU bus. The big-endian flag controls whether or not the data is stored in little-endian or big-endian format.
### gameboy_cpuwrite
`(GameBoy *gb, int address, u8 value) -> void`  
Writes a single byte to the CPU bus.  
There is no cycle cost to any of these external CPU writes.
### gameboy_cpuwritepacked
`(GameBoy *gb, int address, void *src, int size, bool big_endian) -> void`  
Writes multiple bytes from the CPU bus. The big endian flag controls whether or not to write the data in little-endian or big-endian format.
### gameboy_cpureadsym
`(GameBoy *gb, const char *label) -> u8`  
Reads a single byte from the CPU bus at the specified sym label.  
Please refer to [gameboy_cpuread](#gameboy_cpuread) for more information.
### gameboy_cpureadpackedsym
`(GameBoy *gb, const char *label, void *dest, int size, bool big_endian) -> void`  
Reads multiple bytes from the CPU bus at the specified sym label.  
Please refer to [gameboy_cpuread](#gameboy_cpuread)packed for more information.
### gameboy_cpuwritesym
`(GameBoy *gb, const char *label, u8 value) -> void`  
Writes a single byte to the CPU bus at the specified sym label.  
Please refer to [gameboy_cpuwrite](#gameboy_cpuwrite) for more information.
### gameboy_cpuwritepackedsym
`(GameBoy *gb, const char *label, void *src, int size, bool big_endian) -> void`  
Writes multiple bytes from the CPU bus at the specified sym label.  
Please refer to [gameboy_cpuwrite](#gameboy_cpuwrite)packed for more information.
### gameboy_getromtitle
`(GameBoy *gb, char *dest) -> void`  
Writes a null terminated string containing the ROM title to the destination buffer.  
The buffer must at be at least 17 bytes long.
### gameboy_getstatesize
`(GameBoy *gb) -> int`  
Returns the size (in bytes) of the states written by [gameboy_savestate](#gameboy_savestate).
### gameboy_savestate
`(GameBoy *gb, u8 *state) -> void`  
Writes the emulator state to the provided buffer. The optional video snapshot is chosen to be excluded for now. Please use [gameboy_getstatesize](#gameboy_getstatesize) to query the size that the buffer should have.
### gameboy_loadstate
`(GameBoy *gb, u8 *state, int size) -> bool`  
Loads the emulator state from the provided buffer. Returns whether or not the loading was successful.
### gameboy_savestatetodisk
`(GameBoy *gb, const char *filename) -> bool`  
Writes the emulator state to the specified file location. Returns if the file write was successful. For more information please see [gameboy_savestate](#gameboy_savestate).
### gameboy_loadstatefromdisk
`(GameBoy *gb, const char *filename) -> bool`  
Loads the emulator state to the specified file location. Returns if the file read was successful.
### gameboy_getregisters
`(GameBoy *gb) -> Registers`  
Returns the current register values as 32-bit integers. The order of these values are as following: program counter, stack pointer, A, B, C, D, E, F, H, L.
### gameboy_setregisters
`(GameBoy *gb, Registers *regs) -> void`  
Overwrites the current register values.
