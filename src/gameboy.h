#ifndef GAMEBOY_H
#define GAMEBOY_H

#include <stdint.h>
#include <string.h>
#include <string>
#include <map>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef float r32;
typedef double r64;

#define DLLEXPORT extern "C" __declspec(dllexport)
#define DLLIMPORT extern "C"

constexpr u8 NONE = 0x00;
constexpr u8 A = 0x01;
constexpr u8 B = 0x02;
constexpr u8 SELECT = 0x04;
constexpr u8 START = 0x08;
constexpr u8 RIGHT = 0x10;
constexpr u8 LEFT = 0x20;
constexpr u8 UP = 0x40;
constexpr u8 DOWN = 0x80;

constexpr int VIDEO_WIDTH = 160;
constexpr int VIDEO_HEIGHT = 144;
constexpr int VIDEO_BPP = 4;
constexpr int VIDEO_SIZE = VIDEO_WIDTH * VIDEO_HEIGHT;
constexpr int AUDIO_SAMPLES_PER_FRAME = 35112;
constexpr int AUDIO_OVERHEAD = 2064;
constexpr int AUDIO_CHANNELS = 2;
constexpr int AUDIO_BITDEPTH = 2;
constexpr int AUDIO_SIZE = (AUDIO_SAMPLES_PER_FRAME + AUDIO_OVERHEAD) * AUDIO_CHANNELS * AUDIO_BITDEPTH;
constexpr int MAX_BREAKPOINTS = 32;

enum LoadFlags {
    CGB_MODE     = 0x01,
    GBA_FLAG     = 0x02,
    MULIT_COMPAT = 0x04,
    SGB_MODE     = 0x08,
    READONLY_SAV = 0x10,
    NO_BIOS      = 0x20
};

enum LoadResult {
    BAD_FILE_OR_UNKNOWN_MBC = -0x7FFF,
	IO_ERROR,
	UNSUPPORTED_MBC_TAMA5,
	UNSUPPORTED_MBC_MBC7 = -0x122,
	UNSUPPORTED_MBC_MBC6 = -0x120,
	UNSUPPORTED_MBC_MMM01 = -0x10D,
	UNSUPPORTED_MBC_EMS_MULTICART,
	UNSUPPORTED_MBC_BUNG_MULTICART,
	OK = 0
};

enum SpeedupFlags {
    NO_SOUND    = 0x01,
    NO_PPU_CALL = 0x02,
    NO_VIDEO    = 0x04
};

enum Game {
    RED     = 0x01,
    BLUE    = 0x02,
    YELLOW  = 0x04,
    GOLD    = 0x08,
    SILVER  = 0x10,
    CRYSTAL = 0x20,
    
    // NOTE(stringflow): Can bitwise AND against these for checking against multiple games at once.
    RB      = RED  | BLUE,
    RBY     = RED  | BLUE   | YELLOW,
    GS      = GOLD | SILVER,
    GSC     = GOLD | SILVER | CRYSTAL
};

#pragma pack(push, 1)
struct Registers {
    int program_counter;
    int stack_pointer;
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    int h;
    int l;
};
#pragma pack(pop)

struct GameBoy;

#include "utility.h"
#include "symbol-table.h"
#include "gamedata.h"
#include "video.h"
#include "grid-overlay.h"
#include "execution.h"

typedef int (*InputGetter)(void *);
typedef void (*BreakpointCallback)(GameBoy *);
typedef void (*VideoCallback)(GameBoy *, int *);

DLLIMPORT int gambatte_revision();
DLLIMPORT void * gambatte_create();
DLLIMPORT void gambatte_destroy(void *gb);
DLLIMPORT int gambatte_load(void *gb, const char *romfile, int flags);
DLLIMPORT int gambatte_loadbios(void *gb, const char *biosfile, int size, int crc);
DLLIMPORT void gambatte_setinputgetter(void *gb, InputGetter get_input, void *p);
DLLIMPORT void gambatte_setspeedupflags(void *gb, int speedup_flags);
DLLIMPORT int gambatte_timenow(void *gb);
DLLIMPORT void gambatte_setinterruptaddresses(void *gb, int *addrs, int addrs_count);
DLLIMPORT int gambatte_gethitinterruptaddress(void *gb);
DLLIMPORT int gambatte_runfor(void *gb, int *video_buffer, int pitch, u8 *audio_buffer, u64 *samples);
DLLIMPORT void gambatte_getregs(void *gb, Registers *regs);
DLLIMPORT void gambatte_setregs(void *gb, Registers *regs);
DLLIMPORT int gambatte_savestate(void *gb, int *video_buffer, int pitch, u8 *state_buffer);
DLLIMPORT bool gambatte_loadstate(void *gb, u8 *state_buffer, int size);
DLLIMPORT void gambatte_reset(void *gb, int samples_to_stall);
DLLIMPORT void gambatte_cpuwrite(void *gb, int address, u8 value);
DLLIMPORT u8 gambatte_cpuread(void *gb, int address);

// TODO(stringflow): how would people wrap this struct when there are std::maps? Maybe
// should heap allocate those and store pointers instead, or implement my own hash table.
// If going for the 2nd option, can drop all the c++ stdlib. Hash table for strings, can
// have global breakpoints be a linked list.
struct GameBoy {
    void *gambatte_handle;
    int *video_buffer;
    int *video_buffer_copy;
    u8 *audio_buffer;
    u64 buffer_samples;
    u64 samples_emulated;
    int current_buttons;
    int speedup_flags;
    bool callbacks_disabled;
    
    int breakpoint_buffer[MAX_BREAKPOINTS];
    int total_breakpoints;
    std::map<int, BreakpointCallback> global_breakpoints;
    int *temp_breakpoints;
    int temp_breakpoints_size;
    
    Game game;
    GameData gamedata;
    
    FFMPEGPipe *recording;
    Grid grid;
    bool show_grid;
    VideoCallback video_callback;
    
    std::map<std::string, int> state_offsets;
    int state_size;
    u8 *state_buffer;
};

#endif //GAMEBOY_H
