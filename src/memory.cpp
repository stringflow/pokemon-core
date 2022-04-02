// Reads a single byte from the CPU bus. This includes all RAM, ROM, MMIO,
// etc. as it is visible to the CPU (including mappers).
// There is no cycle cost to any of these external CPU reads.
DLLEXPORT u8 gameboy_cpuread(GameBoy *gb, int address) {
    return gambatte_cpuread(gb->gambatte_handle, address);
}

// Reads multiple bytes from the CPU bus. The big-endian flag controls whether or not the
// data is stored in little-endian or big-endian format.
DLLEXPORT void gameboy_cpureadpacked(GameBoy *gb, int address, void *dest, int size, bool big_endian) {
    for(int i = 0; i < size; i++) {
        int address_offset = big_endian ? size-i-1 : i;
        ((u8 *) dest)[i] = gambatte_cpuread(gb->gambatte_handle, address + address_offset);
    }
}

// Writes a single byte to the CPU bus.
// There is no cycle cost to any of these external CPU writes.
DLLEXPORT void gameboy_cpuwrite(GameBoy *gb, int address, u8 value) {
    gambatte_cpuwrite(gb->gambatte_handle, address, value);
}

// Writes multiple bytes from the CPU bus. The big endian flag controls whether or not to
// write the data in little-endian or big-endian format.
DLLEXPORT void gameboy_cpuwritepacked(GameBoy *gb, int address, void *src, int size, bool big_endian) {
    for(int i = 0; i < size; i++) {
        int address_offset = big_endian ? size-i-1 : i;
        gambatte_cpuwrite(gb->gambatte_handle, address + address_offset, ((u8 *) src)[i]);
    }
}

// Reads a single byte from the CPU bus at the specified sym label. 
// Please refer to gameboy_cpuread for more information.
DLLEXPORT u8 gameboy_cpureadsym(GameBoy *gb, const char *label) {
    return gameboy_cpuread(gb, translate_symbol_label(gb, label));
}

// Reads multiple bytes from the CPU bus at the specified sym label.
// Please refer to gameboy_cpureadpacked for more information.
DLLEXPORT void gameboy_cpureadpackedsym(GameBoy *gb, const char *label, void *dest, int size, bool big_endian) {
    gameboy_cpureadpacked(gb, translate_symbol_label(gb, label), dest, size, big_endian);
}

// Writes a single byte to the CPU bus at the specified sym label.
// Please refer to gameboy_cpuwrite for more information.
DLLEXPORT void gameboy_cpuwritesym(GameBoy *gb, const char *label, u8 value) {
    gameboy_cpuwrite(gb, translate_symbol_label(gb, label), value);
}

// Writes multiple bytes from the CPU bus at the specified sym label.
// Please refer to gameboy_cpuwritepacked for more information.
DLLEXPORT void gameboy_cpuwritepackedsym(GameBoy *gb, const char *label, void *src, int size, bool big_endian) {
    gameboy_cpuwritepacked(gb, translate_symbol_label(gb, label), src, size, big_endian);
}

// Writes a null terminated string containing the ROM title to the destination buffer.
// The buffer must at be at least 17 bytes long.
DLLEXPORT void gameboy_getromtitle(GameBoy *gb, char *dest) {
    for(int i = 0; i < 0x10; i++) {
        dest[i] = gameboy_cpuread(gb, 0x134 + i);
    }
    
    dest[dest[0xf] & 0x80 ? 0xf : 0x10] = 0;
}

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

// NOTE(stringflow): DLL INTERNAL
u8 gameboy_cpuread(GameBoy *gb, const char *label) {
    return gameboy_cpureadsym(gb, label);
}

// NOTE(stringflow): DLL INTERNAL
void gameboy_cpuwrite(GameBoy *gb, const char *label, u8 value) {
    return gameboy_cpuwritesym(gb, label, value);
}

// NOTE(stringflow): DLL INTERNAL
u16 gameboy_cpuread16le(GameBoy *gb, int address) {
    u16 result = 0;
    gameboy_cpureadpacked(gb, address, &result, 2, false);
    return result;
}

// NOTE(stringflow): DLL INTERNAL
u16 gameboy_cpuread16be(GameBoy *gb, int address) {
    u16 result = 0;
    gameboy_cpureadpacked(gb, address, &result, 2, true);
    return result;
}

// NOTE(stringflow): DLL INTERNAL
u16 gameboy_cpuread16le(GameBoy *gb, const char *label) {
    return gameboy_cpuread16le(gb, translate_symbol_label(gb, label));
}

// NOTE(stringflow): DLL INTERNAL
u16 gameboy_cpuread16be(GameBoy *gb, const char *label) {
    return gameboy_cpuread16be(gb, translate_symbol_label(gb, label));
}