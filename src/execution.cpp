constexpr int BIOS_JOYPAD = 0x21d;

// Returns an RTC-based timestamp that can be used for time-interval measurements.
// The frequency of this timestamp is 2^21/sec.
// Because this function is based on the real time clock, the output will **not** increase
// linearly when state loads are involved.
DLLEXPORT u64 gameboy_timestamp(GameBoy *gb) {
    return gambatte_timenow(gb->gambatte_handle);
}

// Returns the number of samples that the gameboy reference has emulated. (2^21/sec)
// Note that this number remains **unchanged** when states are loaded.
DLLEXPORT u64 gameboy_samplesemulated(GameBoy *gb) {
    return gb->samples_emulated;
}

// Overwrites the total seconds that have been elapsed since RTC started.
DLLEXPORT void gameboy_setrtc(GameBoy *gb, u32 total_seconds) {
    gameboy_savestate(gb, gb->state_buffer);
    
    if(gambatte_revision() >= 739) {
        u32 seconds_to_time[] = { 24*3600, 3600, 60, 1 };
        constexpr int times_size = ARRAY_SIZE(seconds_to_time);
        int times[times_size];
        
        for(int i = 0; i < times_size; i++) {
            times[i] = total_seconds / seconds_to_time[i];
            total_seconds %= seconds_to_time[i];
        }
        
        gb->state_buffer[gb->state_offsets["rtcdh"]] = times[0] >> 9;
        gb->state_buffer[gb->state_offsets["rtcdl"]] = times[0] & 0xff;
        gb->state_buffer[gb->state_offsets["rtch"]] = times[1] & 0x1f;
        gb->state_buffer[gb->state_offsets["rtcm"]] = times[2] & 0x3f;
        gb->state_buffer[gb->state_offsets["rtcs"]] = times[3] & 0x3f;
    } else {
        endian_swap(&total_seconds);
        int *time_sec = (int *) (gb->state_buffer + gb->state_offsets["timesec"]);
        *time_sec = total_seconds;
    }
    gameboy_loadstate(gb, gb->state_buffer, gb->state_size);
}

// Returns the program counter along with an optional ROM bank. The format is 0xBBAAAA
// where AAAA is an address and BB is an optional ROM bank.
DLLEXPORT int gameboy_getbankedprogramcounter(GameBoy *gb) {
    Registers regs = gameboy_getregisters(gb);
    int bank = 0;
    
    if(regs.program_counter >= 0x4000) {
        bank = gameboy_cpuread(gb, gb->gamedata.rom_bank_address);
    }
    
    return bank << 16 | regs.program_counter;
}

// Sets flags to control non-critical processes for CPU-concerned emulation.
// The flags are defined [here](https://github.com/pokemon-speedrunning/gambatte-core/blob/master/libgambatte/include/gambatte.h#L439-L443).
DLLEXPORT void gameboy_setspeedupflags(GameBoy *gb, int flags) {
    gb->speedup_flags = flags;
    gambatte_setspeedupflags(gb->gambatte_handle, flags);
}

// Returns the flags that result in maximum performance for the loaded game without
// breaking overall emulation.
DLLEXPORT int gameboy_maxspeedupflags(GameBoy *gb) {
    return gb->gamedata.max_speedup_flags;
}

// NOTE(stringflow): Writes the global breakpoints (those with callbacks) and specified
// temporary breakpoints to the breakpoint buffer. Global breakpoints will always
// preceed the temporary ones in the buffer.
void update_breakpoint_buffer(GameBoy *gb) {
    gb->total_breakpoints = 0;
    if(!gb->callbacks_disabled) {
        for(std::pair<int, BreakpointCallback> element : gb->permanent_breakpoints) {
            gb->breakpoint_buffer[gb->total_breakpoints++] = element.first;
        }
    }
    
    if(gb->temp_breakpoints) {
        memcpy(gb->breakpoint_buffer + gb->total_breakpoints, gb->temp_breakpoints, gb->temp_breakpoints_size*4);
        gb->total_breakpoints += gb->temp_breakpoints_size;
    }
    
    gambatte_setinterruptaddresses(gb->gambatte_handle, gb->breakpoint_buffer, gb->total_breakpoints);
}

// Sets a permanent breakpoint at the specified address. Once that address has been hit, the
// provided callback will be called. After that, the breakpoint will still be active and
// will be called again if the address is hit another time.
// For one time breakpoints, make sure to call gameboy_removebreakpoint to
// disable it. Please note that due to a current limitation, there can only be one
// global breakpoint at a given address.
DLLEXPORT void gameboy_setbreakpoint(GameBoy *gb, int address, BreakpointCallback callback) {
    gb->permanent_breakpoints[address] = callback;
    update_breakpoint_buffer(gb);
}

// Removes the permanent breakpoint at the specified address.
DLLEXPORT void gameboy_removebreakpoint(GameBoy *gb, int address) {
    gb->permanent_breakpoints.erase(address);
    update_breakpoint_buffer(gb);
}

// Sets a global breakpoint at the specified sym label.
// Please refer to gameboy_setbreakpoint for more information.
DLLEXPORT void gameboy_setbreakpointsym(GameBoy *gb, const char *label, BreakpointCallback callback) {
    gameboy_setbreakpoint(gb, translate_symbol_label(gb, label), callback);
}

// Removes a global breakpoint at the specified sym label.
// Please refer to gameboy_removebreakpoint for more information.
DLLEXPORT void gameboy_removebreakpointsym(GameBoy *gb, const char *label) {
    gameboy_removebreakpoint(gb, translate_symbol_label(gb, label));
}

// TODO(stringflow): should this be a dll export? i don't think so.
int gameboy_runfor(GameBoy *gb, u64 samples) {
    int vidframe_done_sample_count = gambatte_runfor(gb->gambatte_handle, gb->video_buffer, VIDEO_WIDTH, gb->audio_buffer, &samples);
    int samples_ran = gb->buffer_samples;
    bool vidframe_done = vidframe_done_sample_count >= 0;
    samples_ran += vidframe_done ? vidframe_done_sample_count : samples;
    gb->buffer_samples += samples;
    gb->buffer_samples -= samples_ran;
    gb->samples_emulated += samples_ran;
    
    if(vidframe_done && !gb->callbacks_disabled) {
        if(gb->show_grid && gb->grid.pixels) {
            grid_update_viewport(gb, &gb->grid);
            grid_blittovideo(gb, gb->video_buffer, &gb->grid);
        }
        
        if(gb->video_callback) {
            gb->video_callback(gb, gb->video_buffer);
        }
    }
    
    if(gb->recording && !gb->callbacks_disabled) {
        submit_to_recording(gb->recording,
                            vidframe_done ? gb->video_buffer : nullptr,
                            gb->audio_buffer, samples_ran,
                            gb->samples_emulated);
    }
    
    int hit_address = gambatte_gethitinterruptaddress(gb->gambatte_handle);
    // NOTE(stringflow): don't bother searching for callbacks if CPU wasn't interrupted.
    if(hit_address != -1 && !gb->callbacks_disabled) {
        std::map<int, BreakpointCallback>::iterator it = gb->permanent_breakpoints.find(hit_address);
        if(it != gb->permanent_breakpoints.end()) {
            it->second(gb);
            // NOTE(stringflow): Advance by one sample without any interrupts to ensure
            // infinite loops do not happen (runfor stopping at breakpoint for callback,
            // program counter hasn't advanced before the next runfor = CPU interrupting
            // immediately to call the callback again, etc.)
            // TODO(stringflow): maybe this logic should be done at the top of this function
            // in the niche case that there is both a global and a temporary breakpoint?
            gambatte_setinterruptaddresses(gb->gambatte_handle, nullptr, 0);
            gameboy_runfor(gb, 1);
            gambatte_setinterruptaddresses(gb->gambatte_handle, gb->breakpoint_buffer, gb->total_breakpoints);
        }
    }
    
    return hit_address;
}

// Emulates until the next audio sample has been produced. Will hold the specified buttons
// for the duration of this process.
DLLEXPORT int gameboy_step(GameBoy *gb, int buttons) {
    gb->current_buttons = buttons;
    return gameboy_runfor(gb, 1);
}

// Emulates until the next video frame has been drawn. Will hold the specified buttons
// for the duration of this process.
DLLEXPORT int gameboy_advanceframe(GameBoy *gb, int buttons) {
    gb->current_buttons = buttons;
    return gameboy_runfor(gb, AUDIO_SAMPLES_PER_FRAME - gb->buffer_samples);
}

// Emulates until the next `count` video frames have been drawn. Will hold the specified
// buttons for the duration of this process.
DLLEXPORT int gameboy_advanceframes(GameBoy *gb, int buttons, int count) {
    int ret = 0;
    for(int i = 0; i < count; i++) {
        ret = gameboy_advanceframe(gb, buttons);
    }
    
    return ret;
}

// Emulates until the CPU reaches any of the specified addresses. The format of the
// addresses is 0xBBAAAA where AAAA is the address and BB is an optional ROM bank.
// The address that the CPU stopped at will be returned. Will hold the specified
// buttons for the duration of this process.
DLLEXPORT int gameboy_rununtil(GameBoy *gb, int *addresses, int addresses_size, int buttons) {
    gb->temp_breakpoints = addresses;
    gb->temp_breakpoints_size = addresses_size;
    update_breakpoint_buffer(gb);
    
    int hit_address;
    do {
        hit_address = gameboy_advanceframe(gb, buttons);
        // NOTE(stringflow): Can't check for `hitAddress != -1` here since a global
        // breakpoint might have been hit, in which case we want to continuerunning.
    } while(!in_array(addresses, addresses_size, hit_address));
    
    gb->temp_breakpoints = nullptr;
    gb->temp_breakpoints_size = 0;
    update_breakpoint_buffer(gb);
    return hit_address;
}

// Emulates until the CPU reaches any of the specified sym labels.
// Please refer to gameboy_rununtil for more information.
DLLEXPORT int gameboy_rununtilsym(GameBoy *gb, const char **labels, int labels_size, int buttons) {
    int translation_buffer[MAX_BREAKPOINTS];
    for(int i = 0; i < labels_size; i++) {
        translation_buffer[i] = translate_symbol_label(gb, labels[i]);
    }
    return gameboy_rununtil(gb, translation_buffer, labels_size, buttons);
}

// NOTE(stringflow): DLL INTERNAL
int gameboy_rununtil(GameBoy *gb, const char **labels, int labels_size, int buttons) {
    return gameboy_rununtilsym(gb, labels, labels_size, buttons);
}

// NOTE(stringflow): DLL INTERNAL
int gameboy_rununtil(GameBoy *gb, const char *label, int buttons) {
    return gameboy_rununtilsym(gb, &label, 1, buttons);
}

// NOTE(stringflow): DLL INTERNAL
int gameboy_rununtil(GameBoy *gb, int addr, int buttons) {
    return gameboy_rununtil(gb, &addr, 1, buttons);
}

// Hard resets the emulator. The fade flag controls if the emulator should stall to mimick
// the timing of the Game Boy Player's fade when hard resetting.
DLLEXPORT void gameboy_hardreset(GameBoy *gb, bool fade) {
    // NOTE(stringflow): magic number for fade samples stolen from GSR
    // https://github.com/pokemon-speedrunning/gambatte-speedrun/blob/master/gambatte_qt/src/psrdata.h#L81
    gambatte_reset(gb->gambatte_handle, fade ? 101 * (2 << 14) : 0);
}

// Performs a soft reset of the game. (A B START SELECT)
DLLEXPORT void gameboy_softreset(GameBoy *gb) {
    gameboy_rununtil(gb, "SoftReset", A | B | START | SELECT);
}

// Injects the specified buttons by writing to the game's designated joypad address.
DLLEXPORT void gameboy_injectinput(GameBoy *gb, u8 buttons) {
    if(gb->game & RBY) {
        rby_injectinput(gb, buttons);
    } else {
        gsc_injectinput(gb, buttons);
    }
}

// Runs until the game next polls for joypad input, then injects the input.
DLLEXPORT void gameboy_press(GameBoy *gb, u8 buttons) {
    if(gb->game & RBY) {
        return rby_press(gb, buttons);
    } else {
        return gsc_press(gb, buttons);
    }
}

// Executes any manip log string (i.e. `nopal`, `hop2`, `title1(reset)`, `R`, `D+A`, etc.)
// For overworld actions it returns an enum for when emulation stopped.
// Possible values are: INTRO (0), OVERWORLD_LOOP (1), WILD_ENCOUNTER (2), COLLISION (3),
// TEXTBOX (4). Please note that for gen 2 execution, DVs of the wild encounter will not
// be generated yet, so further emulation until `CalcMonStats` has to be done for those.
DLLEXPORT ExecutionResult gameboy_execute(GameBoy *gb, const char *logstring) {
    if(gb->game & RBY) {
        return rby_execute(gb, logstring);
    } else {
        return gsc_execute(gb, logstring);
    }
}

// Clears all text until control is resumed to the player. This may happen at YES/NO
// dialogues, when overworld movement is resumed, etc. It will hold the specified button
// while text is printing for adequate manip execution, or no button at all for instant
// text execution.
DLLEXPORT void gameboy_cleartext(GameBoy *gb, int held_button) {
    if(gb->game & RBY) {
        rby_cleartext(gb, held_button);
    } else {
        gsc_cleartext(gb, held_button);
    }
}


// Picks up the item in front of the player.
DLLEXPORT void gameboy_pickupitem(GameBoy *gb) {
    if(gb->game & RBY) {
        rby_pickupitem(gb);
    } else {
        gsc_pickupitem(gb);
    }
}

// Executes a full intro string, where each action is seperated by an underscore.
DLLEXPORT void gameboy_executeintro(GameBoy *gb, const char *intro) {
    StringIterator it = { intro };
    while(next_element(&it, '_')) {
        gameboy_execute(gb, it.current_element);
    }
}

// Executes a full movement string, where actions are optionally seperated by spaces.
// May not complete the entire path if a disturbance occurrs (wild encounter, collision,
// etc). Returns the address of the final gameboy_execute call.
DLLEXPORT ExecutionResult gameboy_executepath(GameBoy *gb, const char *path) {
    if(gb->show_grid) {
        if(gb->grid.pixels) {
            grid_free(&gb->grid);
        }
        gb->grid = grid_create(gb, path);
    }
    
    StringIterator it = { path };
    ExecutionResult result = OVERWORLD_LOOP;
    while(iterate_over_path(&it) && result == OVERWORLD_LOOP) {
        result = gameboy_execute(gb, it.current_element);
    }
    return result;
}

// Throws the ball located at the top of the item bag. Returns whether or not the pokemon
// was captured.
// TODO(stringflow): Better way of interfacing this. Select ball, item slot, custom button
//                   presses, etc.
DLLEXPORT bool gameboy_yoloball(GameBoy *gb) {
    gameboy_cleartext(gb, B);
    if(gb->game & RB) {
        gameboy_press(gb, DOWN | A);
        gameboy_press(gb, UP | A);
    } else {
        gameboy_press(gb, DOWN);
        gameboy_press(gb, A);
        gameboy_press(gb, A | B);
    }
    const char *labels[] { "ItemUseBall.captured", "ItemUseBall.failedToCapture" };
    constexpr int labels_size = ARRAY_SIZE(labels);
    return gameboy_rununtil(gb, labels, labels_size, A) == symbol_lookup(gb, labels[0]);
}