# execution.cpp
## Table of Contents
- [gameboy_advanceframe](#gameboy_advanceframe)
- [gameboy_advanceframes](#gameboy_advanceframes)
- [gameboy_cleartext](#gameboy_cleartext)
- [gameboy_execute](#gameboy_execute)
- [gameboy_executeintro](#gameboy_executeintro)
- [gameboy_executepath](#gameboy_executepath)
- [gameboy_getbankedprogramcounter](#gameboy_getbankedprogramcounter)
- [gameboy_hardreset](#gameboy_hardreset)
- [gameboy_injectinput](#gameboy_injectinput)
- [gameboy_maxspeedupflags](#gameboy_maxspeedupflags)
- [gameboy_pickupitem](#gameboy_pickupitem)
- [gameboy_press](#gameboy_press)
- [gameboy_removebreakpoint](#gameboy_removebreakpoint)
- [gameboy_removebreakpointsym](#gameboy_removebreakpointsym)
- [gameboy_rununtil](#gameboy_rununtil)
- [gameboy_rununtilsym](#gameboy_rununtilsym)
- [gameboy_samplesemulated](#gameboy_samplesemulated)
- [gameboy_setbreakpoint](#gameboy_setbreakpoint)
- [gameboy_setbreakpointsym](#gameboy_setbreakpointsym)
- [gameboy_setrtc](#gameboy_setrtc)
- [gameboy_setspeedupflags](#gameboy_setspeedupflags)
- [gameboy_softreset](#gameboy_softreset)
- [gameboy_step](#gameboy_step)
- [gameboy_timestamp](#gameboy_timestamp)
- [gameboy_yoloball](#gameboy_yoloball)
## Functions
### gameboy_timestamp
`(GameBoy *gb) -> u64`  
Returns an RTC-based timestamp that can be used for time-interval measurements.  
The frequency of this timestamp is 2^21/sec.  
Because this function is based on the real time clock, the output will **not** increase linearly when state loads are involved.
### gameboy_samplesemulated
`(GameBoy *gb) -> u64`  
Returns the number of samples that the gameboy reference has emulated. (2^21/sec)  
Note that this number remains **unchanged** when states are loaded.
### gameboy_setrtc
`(GameBoy *gb, u32 total_seconds) -> void`  
Overwrites the total seconds that have been elapsed since RTC started.
### gameboy_getbankedprogramcounter
`(GameBoy *gb) -> int`  
Returns the program counter along with an optional ROM bank. The format is 0xBBAAAA where AAAA is an address and BB is an optional ROM bank.
### gameboy_setspeedupflags
`(GameBoy *gb, SpeedupFlags flags) -> void`  
Sets flags to control non-critical processes for CPU-concerned emulation.  
The flags are defined [here](https://github.com/pokemon-speedrunning/gambatte-core/blob/master/libgambatte/include/gambatte.h#L439-L443).
### gameboy_maxspeedupflags
`(GameBoy *gb) -> int`  
Returns the flags that result in maximum performance for the loaded game without breaking overall emulation.
### gameboy_setbreakpoint
`(GameBoy *gb, int address, BreakpointCallback callback) -> void`  
Sets a global breakpoint at the specified address. Once that address has been hit, the provided callback will be called. After that, the breakpoint will still be active and will be called again if the address is hit another time.  
For one time breakpoints, make sure to call [gameboy_removebreakpoint](#gameboy_removebreakpoint) to disable it. Please note that due to a current limitation, there can only be one global breakpoint at a given address.
### gameboy_removebreakpoint
`(GameBoy *gb, int address) -> void`  
Removes the global breakpoint at the specified address.
### gameboy_setbreakpointsym
`(GameBoy *gb, const char *label, BreakpointCallback callback) -> void`  
Sets a global breakpoint at the specified sym label.  
Please refer to [gameboy_setbreakpoint](#gameboy_setbreakpoint) for more information.
### gameboy_removebreakpointsym
`(GameBoy *gb, const char *label) -> void`  
Removes a global breakpoint at the specified sym label.  
Please refer to [gameboy_removebreakpoint](#gameboy_removebreakpoint) for more information.
### gameboy_step
`(GameBoy *gb, int buttons) -> int`  
Emulates until the next audio sample has been produced. Will hold the specified buttons for the duration of this process.
### gameboy_advanceframe
`(GameBoy *gb, int buttons) -> int`  
Emulates until the next video frame has been drawn. Will hold the specified buttons for the duration of this process.
### gameboy_advanceframes
`(GameBoy *gb, int buttons, int count) -> int`  
Emulates until the next `count` video frames have been drawn. Will hold the specified buttons for the duration of this process.
### gameboy_rununtil
`(GameBoy *gb, int *addresses, int addresses_size, int buttons) -> int`  
Emulates until the CPU reaches any of the specified addresses. The format of the addresses is 0xBBAAAA where AAAA is the address and BB is an optional ROM bank.  
The address that the CPU stopped at will be returned. Will hold the specified buttons for the duration of this process.
### gameboy_rununtilsym
`(GameBoy *gb, const char **labels, int labels_size, int buttons) -> int`  
Emulates until the CPU reaches any of the specified sym labels.  
Please refer to [gameboy_rununtil](#gameboy_rununtil) for more information.
### gameboy_hardreset
`(GameBoy *gb, bool fade) -> void`  
Hard resets the emulator. The fade flag controls if the emulator should stall to mimick the timing of the Game Boy Player's fade when hard resetting.
### gameboy_softreset
`(GameBoy *gb) -> void`  
Performs a soft reset of the game. (A B START SELECT)
### gameboy_injectinput
`(GameBoy *gb, u8 buttons) -> void`  
Injects the specified buttons by writing to the game's designated joypad address.
### gameboy_press
`(GameBoy *gb, u8 buttons) -> void`  
Runs until the game next polls for joypad input, then injects the input.
### gameboy_execute
`(GameBoy *gb, const char *logstring) -> ExecutionResult`  
Executes any manip log string (i.e. `nopal`, `hop2`, `title1(reset)`, `R`, `D+A`, etc.)  
For overworld actions it returns an enum for when emulation stopped.  
Possible values are: INTRO (0), OVERWORLD_LOOP (1), WILD_ENCOUNTER (2), COLLISION (3),  
TEXTBOX (4). Please note that for gen 2 execution, DVs of the wild encounter will not be generated yet, so further emulation until `CalcMonStats` has to be done for those.
### gameboy_cleartext
`(GameBoy *gb, int held_button) -> void`  
Clears all text until control is resumed to the player. This may happen at YES/NO dialogues, when overworld movement is resumed, etc. It will hold the specified button while text is printing for adequate manip execution, or no button at all for instant text execution.
### gameboy_pickupitem
`(GameBoy *gb) -> void`  
Picks up the item in front of the player.
### gameboy_executeintro
`(GameBoy *gb, const char *intro) -> void`  
Executes a full intro string, where each action is seperated by an underscore.
### gameboy_executepath
`(GameBoy *gb, const char *path) -> ExecutionResult`  
Executes a full movement string, where actions are optionally seperated by spaces.  
May not complete the entire path if a disturbance occurrs (wild encounter, collision, etc). Returns the address of the final [gameboy_execute](#gameboy_execute) call.
### gameboy_yoloball
`(GameBoy *gb) -> bool`  
Throws the ball located at the top of the item bag. Returns whether or not the pokemon was captured.
