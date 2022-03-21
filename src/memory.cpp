// Returns the size (in bytes) of the states written by gameboy_savestate.
DLLEXPORT int gameboy_getstatesize(GameBoy *gb) {
    return gb->state_size;
}

// Writes the emulator state to the provided buffer. The optional video snapshot is chosen
// to be excluded for now. Please use gameboy_getstatesize to query the size
// that the buffer should have.
DLLEXPORT void gameboy_savestate(GameBoy *gb, u8 *state) {
    gambatte_savestate(gb->gambatte_handle, nullptr, VIDEO_WIDTH, state);
}

// Loads the emulator state from the provided buffer. Returns whether or not the loading
// was successful.
DLLEXPORT bool gameboy_loadstate(GameBoy *gb, u8 *state, int size) {
    return gambatte_loadstate(gb->gambatte_handle, state, size);
}

// Writes the emulator state to the specified file location. Returns if the file write
// was successful. For more information please see gameboy_savestate.
DLLEXPORT bool gameboy_savestatetodisk(GameBoy *gb, const char *filename) {
    u8 *state = new u8[gb->state_size];
    gameboy_savestate(gb, state);
    bool success = write_entire_file(filename, state, gb->state_size);
    delete[] state;
    return success;
}

// Loads the emulator state to the specified file location. Returns if the file read was
// successful.
DLLEXPORT bool gameboy_loadstatefromdisk(GameBoy *gb, const char *filename) {
    EntireFile file = read_entire_file(filename);
    if(!file.data) return false;
    
    bool success = gameboy_loadstate(gb, file.data, file.size);
    free_entire_file(&file);
    return success;
}

// Returns the current register values as 32-bit integers. The order of these values are
// as following: program counter, stack pointer, A, B, C, D, E, F, H, L.
DLLEXPORT Registers gameboy_getregisters(GameBoy *gb) {
    Registers regs;
    gambatte_getregs(gb->gambatte_handle, &regs);
    return regs;
}

// Overwrites the current register values.
DLLEXPORT void gameboy_setregisters(GameBoy *gb, Registers *regs) {
    gambatte_setregs(gb->gambatte_handle, regs);
}

// Reads a single byte from the CPU bus. This includes all RAM, ROM, MMIO,
// etc. as it is visible to the CPU (including mappers).
// There is no cycle cost to any of these external CPU reads.
DLLEXPORT u8 gameboy_cpuread(GameBoy *gb, int address) {
    return gambatte_cpuread(gb->gambatte_handle, address);
}

// Reads a big endian 16-bit value from the CPU bus.
DLLEXPORT u16 gameboy_cpuread16be(GameBoy *gb, int address) {
    return ((gameboy_cpuread(gb, address+0) << 8) | 
            (gameboy_cpuread(gb, address+1) << 0));
}

// Reads a little endian 24-bit value from the CPU bus.
DLLEXPORT u16 gameboy_cpuread16le(GameBoy *gb, int address) {
    return ((gameboy_cpuread(gb, address+0) << 0) | 
            (gameboy_cpuread(gb, address+1) << 8));
}

// Reads a big endian 24-bit value from the CPU bus.
DLLEXPORT u32 gameboy_cpuread24be(GameBoy *gb, int address) {
    return ((gameboy_cpuread(gb, address+0) << 16) | 
            (gameboy_cpuread(gb, address+1) <<  8) | 
            (gameboy_cpuread(gb, address+2) <<  0));
}

// Reads a little endian 24-bit value from the CPU bus.
DLLEXPORT u32 gameboy_cpuread24le(GameBoy *gb, int address) {
    return ((gameboy_cpuread(gb, address+0) <<  0) | 
            (gameboy_cpuread(gb, address+1) <<  8) | 
            (gameboy_cpuread(gb, address+2) << 16));
}

// Reads a big endian 32-bit value from the CPU bus.
DLLEXPORT u32 gameboy_cpuread32be(GameBoy *gb, int address) {
    return ((gameboy_cpuread(gb, address+0) << 24) | 
            (gameboy_cpuread(gb, address+1) << 16) | 
            (gameboy_cpuread(gb, address+2) <<  8) | 
            (gameboy_cpuread(gb, address+3) <<  0));
}

// Reads a little endian 32-bit value from the CPU bus.
DLLEXPORT u32 gameboy_cpuread32le(GameBoy *gb, int address) {
    return ((gameboy_cpuread(gb, address+0) <<  0) | 
            (gameboy_cpuread(gb, address+1) <<  8) | 
            (gameboy_cpuread(gb, address+2) << 16) | 
            (gameboy_cpuread(gb, address+3) << 24));
}

// Writes a single byte to the CPU bus.
// There is no cycle cost to any of these external CPU writes.
DLLEXPORT void gameboy_cpuwrite(GameBoy *gb, int address, u8 value) {
    gambatte_cpuwrite(gb->gambatte_handle, address, value);
}

// Writes a big endian 16-bit value to the CPU bus.
DLLEXPORT void gameboy_cpuwrite16be(GameBoy *gb, int address, u16 value) {
    gameboy_cpuwrite(gb, address+0, value >> 8);
    gameboy_cpuwrite(gb, address+1, value & 0xff);
}

// Writes a little endian 16-bit value to the CPU bus.
DLLEXPORT void gameboy_cpuwrite16le(GameBoy *gb, int address, u16 value) {
    gameboy_cpuwrite(gb, address+0, value & 0xff);
    gameboy_cpuwrite(gb, address+1, value >> 8);
}

// Writes a big endian 24-bit value to the CPU bus.
DLLEXPORT void gameboy_cpuwrite24be(GameBoy *gb, int address, u32 value) {
    gameboy_cpuwrite(gb, address+0, value >> 16);
    gameboy_cpuwrite(gb, address+1, value >> 8);
    gameboy_cpuwrite(gb, address+2, value & 0xff);
}

// Writes a little endian 24-bit value to the CPU bus.
DLLEXPORT void gameboy_cpuwrite24le(GameBoy *gb, int address, u32 value) {
    gameboy_cpuwrite(gb, address+0, value & 0xff);
    gameboy_cpuwrite(gb, address+1, value >> 8);
    gameboy_cpuwrite(gb, address+2, value >> 16);
}

// Writes a big endian 32-bit value to the CPU bus.
DLLEXPORT void gameboy_cpuwrite32be(GameBoy *gb, int address, u32 value) {
    gameboy_cpuwrite(gb, address+0, value >> 24);
    gameboy_cpuwrite(gb, address+1, value >> 16);
    gameboy_cpuwrite(gb, address+2, value >> 8);
    gameboy_cpuwrite(gb, address+3, value & 0xff);
}

// Writes a little endian 32-bit value to the CPU bus.
DLLEXPORT void gameboy_cpuwrite32le(GameBoy *gb, int address, u32 value) {
    gameboy_cpuwrite(gb, address+0, value & 0xff);
    gameboy_cpuwrite(gb, address+1, value >> 8);
    gameboy_cpuwrite(gb, address+2, value >> 16);
    gameboy_cpuwrite(gb, address+3, value >> 24);
}

// Reads a single byte from the CPU bus at the specified sym label. 
// Please refer to gameboy_cpuread for more information.
DLLEXPORT u8 gameboy_cpureadsym(GameBoy *gb, const char *label) {
    return gameboy_cpuread(gb, translate_symbol_label(gb, label));
}

// Reads a big endian 16-bit value from the CPU bus at the specified sym label.
// Please refer gameboy_cpuread16be for more information.
DLLEXPORT u16 gameboy_cpuread16besym(GameBoy *gb, const char *label) {
    return gameboy_cpuread16be(gb, translate_symbol_label(gb, label));
}

// Reads a little endian 16-bit value from the CPU bus at the specified sym label.
// Please refer gameboy_cpuread16le for more information.
DLLEXPORT u16 gameboy_cpuread16lesym(GameBoy *gb, const char *label) {
    return gameboy_cpuread16le(gb, translate_symbol_label(gb, label));
}

// Reads a big endian 24-bit value from the CPU bus at the specified sym label.
// Please refer gameboy_cpuread24be for more information.
DLLEXPORT u32 gameboy_cpuread24besym(GameBoy *gb, const char *label) {
    return gameboy_cpuread24be(gb, translate_symbol_label(gb, label));
}

// Reads a little endian 24-bit value from the CPU bus at the specified sym label.
// Please refer gameboy_cpuread24le for more information.
DLLEXPORT u32 gameboy_cpuread24lesym(GameBoy *gb, const char *label) {
    return gameboy_cpuread24le(gb, translate_symbol_label(gb, label));
}

// Reads a big endian 32-bit value from the CPU bus at the specified sym label.
// Please refer gameboy_cpuread32be for more information.
DLLEXPORT u32 gameboy_cpuread32besym(GameBoy *gb, const char *label) {
    return gameboy_cpuread32be(gb, translate_symbol_label(gb, label));
}

// Reads a little endian 32-bit value from the CPU bus at the specified sym label.
// Please refer gameboy_cpuread32le for more information.
DLLEXPORT u32 gameboy_cpuread32lesym(GameBoy *gb, const char *label) {
    return gameboy_cpuread32le(gb, translate_symbol_label(gb, label));
}

// Writes a single byte to the CPU bus at the specified sym label.
// Please refer to gameboy_cpuwrite for more information.
DLLEXPORT void gameboy_cpuwritesym(GameBoy *gb, const char *label, u8 value) {
    gameboy_cpuwrite(gb, translate_symbol_label(gb, label), value);
}

// Writes a big endian 16-bit value to the CPU bus at the specified sym label.
// Please refer to gameboy_cpuwrite16be for more information.
DLLEXPORT void gameboy_cpuwrite16besym(GameBoy *gb, const char *label, u16 value) {
    gameboy_cpuwrite16be(gb, translate_symbol_label(gb, label), value);
}

// Writes a little endian 16-bit value to the CPU bus at the specified sym label.
// Please refer to gameboy_cpuwrite16le for more information.
DLLEXPORT void gameboy_cpuwrite16lesym(GameBoy *gb, const char *label, u16 value) {
    gameboy_cpuwrite16le(gb, translate_symbol_label(gb, label), value);
}

// Writes a big endian 24-bit value to the CPU bus at the specified sym label.
// Please refer to gameboy_cpuwrite24be for more information.
DLLEXPORT void gameboy_cpuwrite24besym(GameBoy *gb, const char *label, u32 value) {
    gameboy_cpuwrite24be(gb, translate_symbol_label(gb, label), value);
}

// Writes a little endian 24-bit value to the CPU bus at the specified sym label.
// Please refer to gameboy_cpuwrite24le for more information.
DLLEXPORT void gameboy_cpuwrite24lesym(GameBoy *gb, const char *label, u32 value) {
    gameboy_cpuwrite24le(gb, translate_symbol_label(gb, label), value);
}

// Writes a big endian 32-bit value to the CPU bus at the specified sym label.
// Please refer to gameboy_cpuwrite32be for more information.
DLLEXPORT void gameboy_cpuwrite32besym(GameBoy *gb, const char *label, u32 value) {
    gameboy_cpuwrite32be(gb, translate_symbol_label(gb, label), value);
}

// Writes a little endian 32-bit value to the CPU bus at the specified sym label.
// Please refer to gameboy_cpuwrite32le for more information.
DLLEXPORT void gameboy_cpuwrite32lesym(GameBoy *gb, const char *label, u32 value) {
    gameboy_cpuwrite32le(gb, translate_symbol_label(gb, label), value);
}

// Writes a null terminated string containing the ROM title to the destination buffer.
// The buffer must at be at least 17 bytes long.
DLLEXPORT void gameboy_getromtitle(GameBoy *gb, char *dest) {
    for(int i = 0; i < 0x10; i++) {
        dest[i] = gameboy_cpuread(gb, 0x134 + i);
    }
    
    dest[dest[0xf] & 0x80 ? 0xf : 0x10] = 0;
    
}

// NOTE(stringflow): DLL INTERNAL
u8 gameboy_cpuread(GameBoy *gb, const char *label) {
    return gameboy_cpureadsym(gb, label);
}

// NOTE(stringflow): DLL INTERNAL
void gameboy_cpuwrite(GameBoy *gb, const char *label, u8 value) {
    return gameboy_cpuwritesym(gb, label, value);
}