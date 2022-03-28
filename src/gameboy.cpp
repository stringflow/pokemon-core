#include "gameboy.h"
#include "symbol-table.cpp"
#include "memory.cpp"
#include "video.cpp"
#include "execution.cpp"
#include "rby-execution.h"
#include "gsc-execution.h"
#include "grid-overlay.cpp"

int gambatte_input_callback(void *gb) {
    return ((GameBoy *) gb)->current_buttons;
}

void select_game_data(GameBoy *gb) {
    char rom_title[0x11];
    gameboy_getromtitle(gb, rom_title);
    switch(hash(rom_title)) {
        case hash("POKEMON RED"): {
            gb->game = RED;
            gb->gamedata = RED_DATA;
        } break;
        case hash("POKEMON BLUE"): {
            gb->game = BLUE;
            gb->gamedata = RED_DATA;
        } break;
        case hash("POKEMON YELLOW"): {
            gb->game = YELLOW;
            gb->gamedata = YELLOW_DATA;
        } break;
        case hash("POKEMON_GLDAAUE"): {
            gb->game = GOLD;
            gb->gamedata = GOLD_DATA;
        } break;
        case hash("POKEMON_SLVAAXE"): {
            gb->game = SILVER;
            gb->gamedata = SILVER_DATA;
        } break;
        case hash("PM_CRYSTAL"): {
            gb->game = CRYSTAL;
            gb->gamedata = CRYSTAL_DATA;
        } break;
    }
}

void parse_state_offsets(GameBoy *gb) {
    gameboy_savestate(gb, gb->state_buffer);
    u8 *at = gb->state_buffer + 6;
    u8 *end = gb->state_buffer + gb->state_size;
    while(at < end) {
        char *label = (char *) at;
        while(*at++);
        
        int size = 0;
        size |= (*at++) << 16;
        size |= (*at++) <<  8;
        size |= (*at++) <<  0;
        
        int offset = (int) (at - gb->state_buffer);
        gb->state_offsets[std::string(label)] = offset;
        at += size;
    }
}

LoadResult hotload_sav(u8 *sram, const char *filename) {
    EntireFile sav = read_entire_file(filename);
    if(!sav.data) {
        return IO_ERROR;
    }
    
    memcpy(sram, sav.data, 0x8000);
    free_entire_file(&sav);
    return OK;
}

// Creates a GameBoy reference. This reference will be passed to very subsequent DLL call
// as an identifier. The reference **has** to be freed with gameboy_free once it is
// no longer needed.
DLLEXPORT GameBoy * gameboy_create() {
    return new GameBoy();
}

// Boots up the emulator. Passing a path to valid BIOS and ROM images is required, where as
// SAV data is optional. SAV data is hot-loaded into the emulator as opposed to an OS-level
// file copy, so it is recommended to not have a physical .sav file accompany the ROM file
// in case emulation is needed to start from a fresh save.
// This function will return 0 on success and a negative value on failure. The error codes
// are defined [here](https://github.com/pokemon-speedrunning/gambatte-core/blob/master/libgambatte/include/loadres.h).
DLLEXPORT LoadResult gameboy_load(GameBoy *gb, const char *bios, const char *rom, const char *sav) {
    gb->gambatte_handle = gambatte_create();
    
    // TODO(stringflow): make this a parameter
    LoadFlags flags = (LoadFlags) (READONLY_SAV | GBA_FLAG | CGB_MODE);
    LoadResult result = OK;
    // TODO(stringflow): bios size and crc?
    if((result = gambatte_loadbios(gb->gambatte_handle, bios, 0, 0)) != OK) return result;
    if((result = gambatte_load(gb->gambatte_handle, rom, flags)) != OK) return result;
    gambatte_setinputgetter(gb->gambatte_handle, &gambatte_input_callback, (void *) gb);
    
    gb->state_size = gambatte_savestate(gb->gambatte_handle, nullptr, 0, nullptr);
    gb->video_buffer = new int[VIDEO_SIZE];
    gb->video_buffer_copy = new int[VIDEO_SIZE];
    gb->audio_buffer = new u8[AUDIO_SIZE];
    gb->state_buffer = new u8[gb->state_size];
    
    select_game_data(gb);
    parse_state_offsets(gb);
    
    if(sav && sav[0]) {
        hotload_sav(gb->state_buffer + gb->state_offsets["sram"], sav);
        gameboy_loadstate(gb, gb->state_buffer, gb->state_size);
    }
    
    return result;
}

// Frees the GameBoy reference and any internal data assosicated with it.
DLLEXPORT void gameboy_free(GameBoy *gb) {
    if(!gb) return;
    
    if(gb->recording) ffmpeg_close_pipe(gb->recording);
    if(gb->grid.pixels) grid_free(&gb->grid);
    if(gb->video_buffer) delete[] gb->video_buffer;
    if(gb->video_buffer_copy) delete[] gb->video_buffer_copy;
    if(gb->audio_buffer) delete[] gb->audio_buffer;
    if(gb->state_buffer) delete[] gb->state_buffer;
    if(gb->gambatte_handle) gambatte_destroy(gb->gambatte_handle);
    delete gb;
}