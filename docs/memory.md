# memory.cpp
## Table of Contents
- [gameboy_cpuread16be](#gameboy_cpuread16be)
- [gameboy_cpuread16besym](#gameboy_cpuread16besym)
- [gameboy_cpuread16le](#gameboy_cpuread16le)
- [gameboy_cpuread16lesym](#gameboy_cpuread16lesym)
- [gameboy_cpuread24be](#gameboy_cpuread24be)
- [gameboy_cpuread24besym](#gameboy_cpuread24besym)
- [gameboy_cpuread24le](#gameboy_cpuread24le)
- [gameboy_cpuread24lesym](#gameboy_cpuread24lesym)
- [gameboy_cpuread32be](#gameboy_cpuread32be)
- [gameboy_cpuread32besym](#gameboy_cpuread32besym)
- [gameboy_cpuread32le](#gameboy_cpuread32le)
- [gameboy_cpuread32lesym](#gameboy_cpuread32lesym)
- [gameboy_cpuread](#gameboy_cpuread)
- [gameboy_cpureadsym](#gameboy_cpureadsym)
- [gameboy_cpuwrite16be](#gameboy_cpuwrite16be)
- [gameboy_cpuwrite16besym](#gameboy_cpuwrite16besym)
- [gameboy_cpuwrite16le](#gameboy_cpuwrite16le)
- [gameboy_cpuwrite16lesym](#gameboy_cpuwrite16lesym)
- [gameboy_cpuwrite24be](#gameboy_cpuwrite24be)
- [gameboy_cpuwrite24besym](#gameboy_cpuwrite24besym)
- [gameboy_cpuwrite24le](#gameboy_cpuwrite24le)
- [gameboy_cpuwrite24lesym](#gameboy_cpuwrite24lesym)
- [gameboy_cpuwrite32be](#gameboy_cpuwrite32be)
- [gameboy_cpuwrite32besym](#gameboy_cpuwrite32besym)
- [gameboy_cpuwrite32le](#gameboy_cpuwrite32le)
- [gameboy_cpuwrite32lesym](#gameboy_cpuwrite32lesym)
- [gameboy_cpuwrite](#gameboy_cpuwrite)
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
### gameboy_cpuread
`(GameBoy *gb, int address) -> u8`  
Reads a single byte from the CPU bus. This includes all RAM, ROM, MMIO, etc. as it is visible to the CPU (including mappers).  
There is no cycle cost to any of these external CPU reads.
### gameboy_cpuread16be
`(GameBoy *gb, int address) -> u16`  
Reads a big endian 16-bit value from the CPU bus.
### gameboy_cpuread16le
`(GameBoy *gb, int address) -> u16`  
Reads a little endian 24-bit value from the CPU bus.
### gameboy_cpuread24be
`(GameBoy *gb, int address) -> u32`  
Reads a big endian 24-bit value from the CPU bus.
### gameboy_cpuread24le
`(GameBoy *gb, int address) -> u32`  
Reads a little endian 24-bit value from the CPU bus.
### gameboy_cpuread32be
`(GameBoy *gb, int address) -> u32`  
Reads a big endian 32-bit value from the CPU bus.
### gameboy_cpuread32le
`(GameBoy *gb, int address) -> u32`  
Reads a little endian 32-bit value from the CPU bus.
### gameboy_cpuwrite
`(GameBoy *gb, int address, u8 value) -> void`  
Writes a single byte to the CPU bus.  
There is no cycle cost to any of these external CPU writes.
### gameboy_cpuwrite16be
`(GameBoy *gb, int address, u16 value) -> void`  
Writes a big endian 16-bit value to the CPU bus.
### gameboy_cpuwrite16le
`(GameBoy *gb, int address, u16 value) -> void`  
Writes a little endian 16-bit value to the CPU bus.
### gameboy_cpuwrite24be
`(GameBoy *gb, int address, u32 value) -> void`  
Writes a big endian 24-bit value to the CPU bus.
### gameboy_cpuwrite24le
`(GameBoy *gb, int address, u32 value) -> void`  
Writes a little endian 24-bit value to the CPU bus.
### gameboy_cpuwrite32be
`(GameBoy *gb, int address, u32 value) -> void`  
Writes a big endian 32-bit value to the CPU bus.
### gameboy_cpuwrite32le
`(GameBoy *gb, int address, u32 value) -> void`  
Writes a little endian 32-bit value to the CPU bus.
### gameboy_cpureadsym
`(GameBoy *gb, const char *label) -> u8`  
Reads a single byte from the CPU bus at the specified sym label.  
Please refer to [gameboy_cpuread](#gameboy_cpuread) for more information.
### gameboy_cpuread16besym
`(GameBoy *gb, const char *label) -> u16`  
Reads a big endian 16-bit value from the CPU bus at the specified sym label.  
Please refer [gameboy_cpuread](#gameboy_cpuread)16be for more information.
### gameboy_cpuread16lesym
`(GameBoy *gb, const char *label) -> u16`  
Reads a little endian 16-bit value from the CPU bus at the specified sym label.  
Please refer [gameboy_cpuread](#gameboy_cpuread)16le for more information.
### gameboy_cpuread24besym
`(GameBoy *gb, const char *label) -> u32`  
Reads a big endian 24-bit value from the CPU bus at the specified sym label.  
Please refer [gameboy_cpuread](#gameboy_cpuread)24be for more information.
### gameboy_cpuread24lesym
`(GameBoy *gb, const char *label) -> u32`  
Reads a little endian 24-bit value from the CPU bus at the specified sym label.  
Please refer [gameboy_cpuread](#gameboy_cpuread)24le for more information.
### gameboy_cpuread32besym
`(GameBoy *gb, const char *label) -> u32`  
Reads a big endian 32-bit value from the CPU bus at the specified sym label.  
Please refer [gameboy_cpuread](#gameboy_cpuread)32be for more information.
### gameboy_cpuread32lesym
`(GameBoy *gb, const char *label) -> u32`  
Reads a little endian 32-bit value from the CPU bus at the specified sym label.  
Please refer [gameboy_cpuread](#gameboy_cpuread)32le for more information.
### gameboy_cpuwritesym
`(GameBoy *gb, const char *label, u8 value) -> void`  
Writes a single byte to the CPU bus at the specified sym label.  
Please refer to [gameboy_cpuwrite](#gameboy_cpuwrite) for more information.
### gameboy_cpuwrite16besym
`(GameBoy *gb, const char *label, u16 value) -> void`  
Writes a big endian 16-bit value to the CPU bus at the specified sym label.  
Please refer to [gameboy_cpuwrite](#gameboy_cpuwrite)16be for more information.
### gameboy_cpuwrite16lesym
`(GameBoy *gb, const char *label, u16 value) -> void`  
Writes a little endian 16-bit value to the CPU bus at the specified sym label.  
Please refer to [gameboy_cpuwrite](#gameboy_cpuwrite)16le for more information.
### gameboy_cpuwrite24besym
`(GameBoy *gb, const char *label, u32 value) -> void`  
Writes a big endian 24-bit value to the CPU bus at the specified sym label.  
Please refer to [gameboy_cpuwrite](#gameboy_cpuwrite)24be for more information.
### gameboy_cpuwrite24lesym
`(GameBoy *gb, const char *label, u32 value) -> void`  
Writes a little endian 24-bit value to the CPU bus at the specified sym label.  
Please refer to [gameboy_cpuwrite](#gameboy_cpuwrite)24le for more information.
### gameboy_cpuwrite32besym
`(GameBoy *gb, const char *label, u32 value) -> void`  
Writes a big endian 32-bit value to the CPU bus at the specified sym label.  
Please refer to [gameboy_cpuwrite](#gameboy_cpuwrite)32be for more information.
### gameboy_cpuwrite32lesym
`(GameBoy *gb, const char *label, u32 value) -> void`  
Writes a little endian 32-bit value to the CPU bus at the specified sym label.  
Please refer to [gameboy_cpuwrite](#gameboy_cpuwrite)32le for more information.
### gameboy_getromtitle
`(GameBoy *gb, char *dest) -> void`  
Writes a null terminated string containing the ROM title to the destination buffer.  
The buffer must at be at least 17 bytes long.
