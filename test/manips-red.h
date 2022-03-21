#ifndef MANIPS_RED_H
#define MANIPS_RED_H

bool red_triple_extended(GameBoy *gb, int path) {
    gameboy_executeintro(gb, "nopal_gfskip_hop0_title0_cont_cont");
    ASSERT_EQUALS(gameboy_executepath(gb,"LLLULLUAULALDLDLLDADDADLALLALUUAU"),
                  symbol_lookup(gb, "CalcStats"));
    ASSERT_EQUALS(gameboy_cpuread(gb, "wCurMap"), 33);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 33);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 11);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wEnemyMonSpecies"), 3);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wEnemyMonLevel"), 4);
    ASSERT_EQUALS(gameboy_cpuread16besym(gb, "wEnemyMonDVs"), 0xffef);
    
    ASSERT_EQUALS(gameboy_yoloball(gb), true);
    
    gameboy_cleartext(gb, B);
    gameboy_press(gb, A);
    gameboy_press(gb, NONE);
    gameboy_press(gb, NONE);
    gameboy_press(gb, NONE);
    gameboy_press(gb, A);
    gameboy_press(gb, START);
    
    ASSERT_EQUALS(gameboy_executepath(gb,"DRRUUURRRRRRRRRRRRRRRRRRRRRURUUUUUURUUAUULUUUAUUUUUUUUUUUUUUAUULLLUUUUUUURRRRUAUUU"),
                  symbol_lookup(gb, "CalcStats"));
    ASSERT_EQUALS(gameboy_cpuread(gb, "wCurMap"), 13);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 8);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 48);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wEnemyMonSpecies"), 36);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wEnemyMonLevel"), 5);
    ASSERT_EQUALS(gameboy_yoloball(gb), true);
    gameboy_cleartext(gb, A);
    gameboy_press(gb, B);
    
    ASSERT_EQUALS(gameboy_executepath(gb,"UUUULLLLLU"),
                  symbol_lookup(gb, "JoypadOverworld"));
    ASSERT_EQUALS(gameboy_executepath(gb,"UUUUURUU"),
                  symbol_lookup(gb, "JoypadOverworld"));
    ASSERT_EQUALS(gameboy_executepath(gb,"UUAUURUARRRRRRRUUUUUUUUUUUAUUAUUUUUUUUUUUUUUUUU"),
                  symbol_lookup(gb, "JoypadOverworld"));
    ASSERT_EQUALS(gameboy_cpuread(gb, "wCurMap"), 51);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 25);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 12);
    gameboy_pickupitem(gb);
    
    ASSERT_EQUALS(gameboy_executepath(gb,"UUULLLLLLLLDDDDDDDLLLLUUUUUUUUUUUUULLLLLLDDDDDDDDDDDDDDDDDDLDLLLLUUU"),
                  symbol_lookup(gb, "JoypadOverworld"));
    ASSERT_EQUALS(gameboy_cpuread(gb, "wCurMap"), 51);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 2);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 19);
    return true;
}


bool red_moon_route3(GameBoy *gb, int path) {
    gameboy_executeintro(gb, "pal(hold)_gfskip_hop0_title0_cont_cont");
    ASSERT_EQUALS(gameboy_executepath(gb,"RRRRRRRRURRUUUUUARRRRRRRRRRRRDDDDDRRRRRRRARUURRUUUUUUUUUURRRRUUUUUUUUUURRRRRU"),
                  symbol_lookup(gb, "JoypadOverworld"));
    ASSERT_EQUALS(gameboy_executepath(gb,"UUUUUULLLLLALLLLDD"),
                  symbol_lookup(gb, "JoypadOverworld"));
    ASSERT_EQUALS(gameboy_cpuread(gb, "wCurMap"), 59);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 5);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 31);
    gameboy_pickupitem(gb);
    
    ASSERT_EQUALS(gameboy_executepath(gb,"RRRRUURRRARRUUUUUUURRRRRRRAUUUUUUURRRDRDDDDDDDADDDDDDDDADRRRRRURRRR"),
                  symbol_lookup(gb, "JoypadOverworld"));
    ASSERT_EQUALS(gameboy_cpuread(gb, "wCurMap"), 59);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 34);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 31);
    gameboy_pickupitem(gb);
    
    ASSERT_EQUALS(gameboy_executepath(gb,"UUUUUUUUR"),
                  symbol_lookup(gb, "JoypadOverworld"));
    ASSERT_EQUALS(gameboy_cpuread(gb, "wCurMap"), 59);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 35);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 23);
    gameboy_pickupitem(gb);
    
    ASSERT_EQUALS(gameboy_executepath(gb,"ULUUUUUAUUUUUULLLUUUUUUUULLLLLLDDLALLLLLLLDDDDDD"),
                  symbol_lookup(gb, "JoypadOverworld"));
    ASSERT_EQUALS(gameboy_executepath(gb,"LALLALLALLALDD"),
                  symbol_lookup(gb, "JoypadOverworld"));
    ASSERT_EQUALS(gameboy_executepath(gb,"RRRUUULAUR"),
                  symbol_lookup(gb, "JoypadOverworld"));
    ASSERT_EQUALS(gameboy_cpuread(gb, "wCurMap"), 61);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 28);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 5);
    gameboy_pickupitem(gb);
    
    ASSERT_EQUALS(gameboy_executepath(gb,"DDADLALLAD"),
                  symbol_lookup(gb, "JoypadOverworld"));
    ASSERT_EQUALS(gameboy_executepath(gb,"RARRARRARRARUU"),
                  symbol_lookup(gb, "JoypadOverworld"));
    ASSERT_EQUALS(gameboy_executepath(gb,"DDLDDDDLLLLLLLULUUUUULUUUUUUUULLLUL"),
                  symbol_lookup(gb, "JoypadOverworld"));
    ASSERT_EQUALS(gameboy_cpuread(gb, "wCurMap"), 59);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 3);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 2);
    gameboy_pickupitem(gb);
    
    ASSERT_EQUALS(gameboy_executepath(gb,"DADDRAR"),
                  symbol_lookup(gb, "JoypadOverworld"));
    ASSERT_EQUALS(gameboy_executepath(gb,"DRRDDDDDDDDDDRRRARRRRRRRRRRDR"),
                  symbol_lookup(gb, "JoypadOverworld"));
    ASSERT_EQUALS(gameboy_executepath(gb,"RRUUURARRRDDRRRRRUARURARRDDDDDDDDALLLLDDDDDDDADDLLLALLLLLLLLLLLLALLLLLLUUUUAUUALUUUUUUUU"),
                  symbol_lookup(gb, "CalcStats"));
    
    ASSERT_EQUALS(gameboy_cpuread(gb, "wCurMap"), 61);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 10);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 17);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wEnemyMonSpecies"), 109);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wEnemyMonLevel"), 10);
    gameboy_cleartext(gb, B);
    ASSERT_EQUALS(gameboy_yoloball(gb), true);
    return true;
}

bool red_cans(GameBoy *gb, int path) {
    gameboy_executeintro(gb, "red_nopal_gfskip_hop0_title0_cont_cont");
    ASSERT_EQUALS(gameboy_executepath(gb,"DALLLAU"),
                  symbol_lookup(gb, "JoypadOverworld"));
    ASSERT_EQUALS(gameboy_executepath(gb,"RUUUUU"),
                  symbol_lookup(gb, "JoypadOverworld"));
    gameboy_press(gb, A);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wCurMap"), 92);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 5);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 12);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wFirstLockTrashCanIndex"), 8);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wSecondLockTrashCanIndex"), 5);
    return true;
}

#endif //MANIPS_RED_H
