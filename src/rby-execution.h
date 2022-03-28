#ifndef RBY_EXECUTION_H
#define RBY_EXECUTION_H

void rby_injectinput(GameBoy *gb, int buttons) {
    gameboy_cpuwrite(gb, "hJoyInput", buttons);
}

void rby_press(GameBoy *gb, int buttons) {
    gameboy_rununtil(gb, "_Joypad", NONE);
    rby_injectinput(gb, buttons);
    gameboy_advanceframe(gb, NONE);
}

ExecutionResult rby_execute(GameBoy *gb, const char *logstring) {
    Action action = parse_action(logstring);
    
    int addr = 0;
    int buttons = 0;
    switch(hash(action.string)) {
        case hash("R"): if(!buttons) buttons = RIGHT;
        case hash("L"): if(!buttons) buttons = LEFT;
        case hash("U"): if(!buttons) buttons = UP;
        case hash("D"): if(!buttons) buttons = DOWN; {
            int overworld_addrs[] {
                symbol_lookup(gb, "HandleLedges.foundMatch"),
                symbol_lookup(gb, "CollisionCheckOnLand.collision"),
                symbol_lookup(gb, "CollisionCheckOnWater.collision"),
                symbol_lookup(gb, "TryDoWildEncounter.CanEncounter")+6,
                symbol_lookup(gb, "OverworldLoopLessDelay.newBattle")+3,
            };
            constexpr int overworld_addrs_size = ARRAY_SIZE(overworld_addrs);
            
            do {
                gameboy_rununtil(gb, "JoypadOverworld", NONE);
                gameboy_injectinput(gb, buttons);
                addr = gameboy_rununtil(gb, overworld_addrs, overworld_addrs_size, buttons);
                if(addr == symbol_lookup(gb, "TryDoWildEncounter.CanEncounter")+6) {
                    gameboy_rununtil(gb, "CalcStats", NONE);
                    return WILD_ENCOUNTER;
                } else if(addr == symbol_lookup(gb, "CollisionCheckOnLand.collision") ||
                          addr == symbol_lookup(gb, "CollisionCheckOnWater.collision")) {
                    return COLLISION;
                }
                addr = gameboy_rununtil(gb, "JoypadOverworld", NONE);
            } while(gameboy_cpuread(gb, "wd736") & 0x40 ||      // Jumping ledge
                    gameboy_cpuread(gb, "wd736") & 0x02 ||      // Door tile pathing
                    gameboy_cpuread(gb, "wd730") & 0x08 ||      // Simualted joypad
                    gameboy_cpuread(gb, "wJoyIgnore") >= 0xfc); // Disabled Joypad
            return OVERWORLD_LOOP;
        } break;
        case hash("A"): {
            int a_press_addrs[] {
                symbol_lookup(gb, "JoypadOverworld"),
                symbol_lookup(gb, "PrintLetterDelay"),
            };
            constexpr int a_press_addrs_size = ARRAY_SIZE(a_press_addrs);
            gameboy_injectinput(gb, A);
            gameboy_runfor(gb, 1);
            addr = gameboy_rununtil(gb, a_press_addrs, a_press_addrs_size, A);
            return addr == a_press_addrs[0] ? OVERWORLD_LOOP : TEXTBOX;
        } break;
        case hash("S_B"): {
            gameboy_press(gb, START);
            gameboy_press(gb, B);
            addr = gameboy_rununtil(gb, "JoypadOverworld", NONE);
            return OVERWORLD_LOOP;
        } break;
        case hash("S_A_B_S"): {
            gameboy_press(gb, START);
            gameboy_press(gb, A);
            gameboy_press(gb, B);
            gameboy_press(gb, START);
            addr = gameboy_rununtil(gb, "JoypadOverworld", NONE);
            return OVERWORLD_LOOP;
        } break;
        case hash("S_A_B_A_B_S"): {
            gameboy_press(gb, START);
            gameboy_press(gb, A);
            gameboy_press(gb, B);
            gameboy_press(gb, A);
            gameboy_press(gb, B);
            gameboy_press(gb, START);
            addr = gameboy_rununtil(gb, "JoypadOverworld", NONE);
        } break;
        case hash("nopal"): {
            gameboy_rununtil(gb, 0x100, NONE);
        } break;
        case hash("nopal(ab)"): {
            gameboy_rununtil(gb, 0x100, A);
        } break;
        case hash("pal"): {
            gameboy_rununtil(gb, BIOS_JOYPAD, NONE);
            gameboy_advanceframe(gb, B | LEFT);
            gameboy_rununtil(gb, 0x100, NONE);
        } break;
        case hash("pal(hold)"): {
            gameboy_rununtil(gb, 0x100, B | LEFT);
        } break;
        case hash("pal(ab)"): {
            gameboy_rununtil(gb, BIOS_JOYPAD, NONE);
            gameboy_advanceframes(gb, 70, LEFT);
            gameboy_rununtil(gb, 0x100, B | LEFT);
        } break;
        case hash("pal(rel)"): {
            gameboy_rununtil(gb, BIOS_JOYPAD, NONE);
            gameboy_advanceframe(gb, LEFT);
            gameboy_advanceframes(gb, 70, NONE);
            gameboy_rununtil(gb, 0x100, B | LEFT);
        } break;
        case hash("gfskip"): {
            gameboy_press(gb, UP | SELECT | B);
        } break;
        case hash("gfwait"): {
            gameboy_rununtil(gb, "PlayShootingStar.next", NONE);
        } break;
        case hash("gfreset"): {
            // NOTE(stringflow): This is for consecutive gfresets.
            gameboy_rununtil(gb, "CheckForUserInterruption", NONE);
            gameboy_softreset(gb);
        } break;
        case hash("hop"): {
            if(action.repetitions >= 6) {
                gameboy_rununtil(gb, "DisplayTitleScreen", NONE);
            } else {
                for(int i = 0; i < action.repetitions; i++) {
                    gameboy_rununtil(gb, "AnimateIntroNidorino", NONE);
                    gameboy_rununtil(gb, "CheckForUserInterruption", NONE);
                }
                gameboy_press(gb, UP | SELECT | B);
            }
        } break;
        case hash("hop(reset)"): {
            for(int i = 0; i < action.repetitions; i++) {
                gameboy_rununtil(gb, "AnimateIntroNidorino", NONE);
                gameboy_rununtil(gb, "CheckForUserInterruption", NONE);
            }
            gameboy_softreset(gb);
        } break;
        case hash("intro"): {
            const char * intro_addrs[] {
                "YellowIntroScene0",
                "YellowIntroScene2",
                "YellowIntroScene4",
                "YellowIntroScene6",
                "YellowIntroScene8",
                "YellowIntroScene10",
                "YellowIntroScene12",
            };
            if(action.repetitions > 0) gameboy_rununtil(gb, intro_addrs[action.repetitions], NONE);
            gameboy_press(gb, A);
        } break;
        case hash("title"): {
            for(int i = 0; i < action.repetitions; i++) {
                gameboy_rununtil(gb, "TitleScreenPickNewMon", NONE);
                gameboy_rununtil(gb, "CheckForUserInterruption", NONE);
            }
            gameboy_press(gb, START);
        } break;
        case hash("title(reset)"): {
            for(int i = 0; i < action.repetitions; i++) {
                gameboy_rununtil(gb, "TitleScreenPickNewMon", NONE);
                gameboy_rununtil(gb, "CheckForUserInterruption", NONE);
            }
            gameboy_softreset(gb);
        } break;
        case hash("title(usb)"): {
            for(int i = 0; i < action.repetitions; i++) {
                gameboy_rununtil(gb, "TitleScreenPickNewMon", NONE);
                gameboy_rununtil(gb, "CheckForUserInterruption", NONE);
            }
            gameboy_press(gb, UP | B | SELECT);
        } break;
        case hash("title(scroll)"): {
            for(int i = 0; i < action.repetitions; i++) {
                gameboy_rununtil(gb, "TitleScreenScrollInMon", NONE);
                gameboy_rununtil(gb, "CheckForUserInterruption", NONE);
            }
            gameboy_press(gb, START);
        } break;
        case hash("title(scrollreset)"): {
            for(int i = 0; i < action.repetitions; i++) {
                gameboy_rununtil(gb, "TitleScreenScrollInMon", NONE);
                gameboy_rununtil(gb, "CheckForUserInterruption", NONE);
            }
            gameboy_softreset(gb);
        } break;
        case hash("title(scrollusb)"): {
            for(int i = 0; i < action.repetitions; i++) {
                gameboy_rununtil(gb, "TitleScreenScrollInMon", NONE);
                gameboy_rununtil(gb, "CheckForUserInterruption", NONE);
            }
            gameboy_press(gb, UP | B | SELECT);
        } break;
        case hash("backout"):
        case hash("fsback"): {
            gameboy_press(gb, B);
        } break;
        case hash("newgame"):
        case hash("cont"): {
            gameboy_press(gb, A);
        } break;
        case hash("opt(backout)"): {
            gameboy_press(gb, DOWN | A);
            gameboy_press(gb, START);
        } break;
        case hash("opt(reset)"): {
            gameboy_press(gb, DOWN | A);
            gameboy_softreset(gb);
        } break;
        case hash("csreset"):
        case hash("ngreset"):
        case hash("oakreset"): {
            gameboy_softreset(gb);
        } break;
        case hash("red"):
        case hash("blue"):
        case hash("yellow"):
        case hash("gold"):
        case hash("silver"):
        case hash("crystal"): {
            // NOTE(stringflow): do nothing, just a manip header.
        } break;
    }
    
    return INTRO;
}

void rby_cleartext(GameBoy *gb, int held_button) {
    int text_addrs[] {
        symbol_lookup(gb, "PrintLetterDelay.checkButtons")+3,
        symbol_lookup(gb, "WaitForTextScrollButtonPress.skipAnimation")+10,
        symbol_lookup(gb, "HoldTextDisplayOpen")+3,
        (symbol_lookup(gb, "ShowPokedexDataInternal.waitForButtonPress") & 0xffff)+3,
        symbol_lookup(gb, "TextCommand_PAUSE")+4,
    };
    constexpr int text_addrs_size = ARRAY_SIZE(text_addrs);
    
    while(true) {
        gameboy_rununtil(gb, "Joypad", held_button);
        int sp = gameboy_getregisters(gb).stack_pointer;
        int came_from = gameboy_cpuread16le(gb, sp);
        
        if(came_from == symbol_lookup(gb, "JoypadLowSensitivity")+3) {
            came_from = gameboy_cpuread16le(gb, sp+2);
        }
        
        if(!in_array(text_addrs, text_addrs_size, came_from)) {
            if(came_from == symbol_lookup(gb, "JoypadOverworld")+13 &&
               (gameboy_cpuread(gb, "wJoyIgnore") >= 0xfc ||
                gameboy_cpuread(gb, "wd730")       & 0xa1 ||
                gameboy_cpuread(gb, "wFlags_D733") & 0x08 ||
                gameboy_cpuread(gb, "wCurOpponent"))) {
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
            u8 previous = gameboy_cpuread(gb, "hJoyLast") & (A|B);
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

void rby_pickupitem(GameBoy *gb) {
    gameboy_press(gb, A);
    gameboy_rununtil(gb, "PlaySound", A);
}

#endif //RBY_EXECUTION_H
