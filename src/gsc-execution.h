#ifndef GSC_EXECUTION_H
#define GSC_EXECUTION_H

void gsc_injectinput(GameBoy *gb, int buttons) {
    // TODO(stringflow): hJoyInput   = input read from the hardware
    //                   hJoyDown    = processed input (current frame)
    //                   hJoyPressed = delta input (compared to last frame)
    //
    // For gen 1, you're able to only have to overwrite hJoyInput before the joypad is
    // processed by the game. By overwriting the input parameters of the joypad processing
    // routine, all quirks (double input, consecutive input blocking, etc.) will be
    // emulated properly.
    //
    // Where gen 2 differs is that for overworld movement, it processes the joypad BEFORE
    // checking if you are even eligible to move. As a result, the output of the joypad
    // processing routine needs to be modified instead. I see multiple ways of dealing
    // with this situation:
    // 1) Have all input injections do this. It's the easiest solution, Since there's no way
    //    for us to compute the real delta input, let's just overwrite it with the joypad we
    //    wanted to inject! Works for now, but may lead to trouble down the road where the
    //    game checks for the delta value and it always being set to the same as the
    //    processed input causes menus to execute faster than actually possible.
    //
    // 2) When injecting, check if program counter == OWPlayerInput, if it is then overwrite
    //    the output values, if it isn't overwrite the input values. This should not cause
    //    any unwanted behavior, but relies on the fact that any overworld input injection
    //    has to have the program counter at that specific position.
    //
    // 3) Make sure that this function is always called before the value for the previous
    //    input is overwritten, so that the delta values can be properly calculated.
    //
    // Let's try #3 for now.
    u8 last = gameboy_cpuread(gb, "hJoyDown");
    gameboy_cpuwrite(gb, "hJoyReleased", (last ^ buttons) & last);
    gameboy_cpuwrite(gb, "hJoyPressed", (last ^ buttons) & buttons);
    // NOTE(stringflow): step instead of edit registers in the case that execution isn't
    // in the GetJoypad routine (for overworld movement).
    gameboy_step(gb, buttons);
    gameboy_cpuwrite(gb, "hJoyDown", buttons);
}

void gsc_press(GameBoy *gb, int buttons) {
    int banked_pc = gameboy_getbankedprogramcounter(gb);
    // NOTE(stringflow): don't run when pc is at OWPlayerInput, see comment above.
    if(banked_pc != symbol_lookup(gb, "OWPlayerInput")) {
        // TODO(stringflow): need to pass something for gold interrupt, ideally the
        // "opposite" of what you're trying to press (B instead of A, 
        // SELECT instead of START, UP instead of DOWN, etc.), maybe a lookup table?
        gameboy_rununtil(gb, "GetJoypad.quit-2", buttons << 1);
    }
    
    gsc_injectinput(gb, buttons);
    gameboy_advanceframe(gb, buttons);
}

ExecutionResult gsc_execute(GameBoy *gb, const char *logstring) {
    Action action = parse_action(logstring);
    
    int addr = 0;
    int buttons = 0;
    switch(hash(action.string)) {
        case hash("R"): if(!buttons) buttons = RIGHT;
        case hash("L"): if(!buttons) buttons = LEFT;
        case hash("U"): if(!buttons) buttons = UP;
        case hash("D"): if(!buttons) buttons = DOWN;
        case hash("A+R"): if(!buttons) buttons = A | RIGHT;
        case hash("A+L"): if(!buttons) buttons = A | LEFT;
        case hash("A+U"): if(!buttons) buttons = A | UP;
        case hash("A+D"): if(!buttons) buttons = A | DOWN; {
            int overworld_addrs[] = {
                symbol_lookup(gb, "CountStep"),
                symbol_lookup(gb, "PrintLetterDelay.checkjoypad"),
                symbol_lookup(gb, "DoPlayerMovement.BumpSound"),
                symbol_lookup(gb, "RandomEncounter.ok"),
                symbol_lookup(gb, "OWPlayerInput"),
            };
            constexpr int overworld_addrs_size = ARRAY_SIZE(overworld_addrs);
            
            gameboy_rununtil(gb, "OWPlayerInput", buttons);
            gameboy_injectinput(gb, buttons);
            addr = gameboy_rununtil(gb, overworld_addrs, overworld_addrs_size-1, buttons);
            if(addr == symbol_lookup(gb, "CountStep")) {
                addr = gameboy_rununtil(gb, overworld_addrs+3, 2, buttons);
            }
            
            // TODO(stringflow): emulating until CalcMonStats is super slow for no encounter
            // manips, so not sure if i wanna do it in here.
            if(addr == overworld_addrs[1]) return TEXTBOX;
            else if(addr == overworld_addrs[2]) return COLLISION;
            else if(addr == overworld_addrs[3]) return WILD_ENCOUNTER;
            return OVERWORLD_LOOP;
        } break;
        case hash("S_B"): {
            gameboy_press(gb, START);
            gameboy_press(gb, B);
            addr = gameboy_rununtil(gb, "OWPlayerInput", B);
            return OVERWORLD_LOOP;
        } break;
        case hash("SEL"): {
            gameboy_press(gb, SELECT);
            addr = gameboy_rununtil(gb, "OWPlayerInput", SELECT);
            return OVERWORLD_LOOP;
        } break;
        case hash("gfskip"): 
        case hash("title"):{
            gameboy_press(gb, START);
        } break;
        case hash("cont"): {
            gameboy_press(gb, A);
        } break;
        case hash("delay"): {
            gameboy_rununtil(gb, "GetJoypad", SELECT);
            gameboy_advanceframes(gb, SELECT, 1+action.repetitions);
        } break;
    }
    
    return INTRO;
}

void gsc_cleartext(GameBoy *gb, int held_button) {
    int text_addrs[] {
        symbol_lookup(gb, "PrintLetterDelay.checkjoypad")+3,
        symbol_lookup(gb, "PromptButton.input_wait_loop")+6,
        symbol_lookup(gb, "WaitPressAorB_BlinkCursor.loop")+11,
        symbol_lookup(gb, "JoyWaitAorB.loop")+6,
        symbol_lookup(gb, "TextCommand_PAUSE")+5,
    };
    constexpr int text_addrs_size = ARRAY_SIZE(text_addrs);
    
    while(true) {
        gameboy_rununtil(gb, gb->gamedata.joypad_address, held_button);
        int sp = gameboy_getregisters(gb).stack_pointer;
        int came_from = gameboy_cpuread16le(gb, sp+8);
        
        if(came_from == symbol_lookup(gb, "JoyTextDelay")+3) {
            came_from = gameboy_cpuread16le(gb, sp+10);
        }
        
        if(!in_array(text_addrs, text_addrs_size, came_from)) {
            if(came_from == (symbol_lookup(gb, "HandleMapTimeAndJoypad") & 0xffff)+12 &&
               (gameboy_cpuread(gb, "wScriptMode") == 2)) {
                // TODO(stringflow): step or advanceframe?
                gameboy_injectinput(gb, held_button);
                gameboy_advanceframe(gb, held_button);
            } else {
                gameboy_step(gb, NONE);
                break;
            }
        }
        
        if(came_from == text_addrs[0]) { 
            gameboy_injectinput(gb, held_button);
            // TODO(stringflow): step or advanceframe?
            gameboy_advanceframe(gb, held_button);
        } else {
            u8 previous = gameboy_cpuread(gb, "hJoyDown") & (A|B);
            u8 text_clear_button;
            if(previous == 0) {
                text_clear_button = came_from == text_addrs[1] ? B : A;
            } else {
                text_clear_button = previous ^ (A|B);
            }
            
            gameboy_injectinput(gb, text_clear_button);
            gameboy_step(gb, text_clear_button);
        }
    }
}

void gsc_pickupitem(GameBoy *gb) {
    gameboy_press(gb, A);
    gameboy_cleartext(gb, A);
}

#endif //GSC_EXECUTION_H
