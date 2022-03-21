#ifndef MANIPS_YELLOW_H
#define MANIPS_YELLOW_H

bool yellow_nido(GameBoy *gb, int path) {
    gameboy_executeintro(gb, "gfskip_intro0_title_cont_cont");
    ASSERT_EQUALS(gameboy_executepath(gb, "URARU"),
                  symbol_lookup(gb, "CalcStats"));
    ASSERT_EQUALS(gameboy_cpuread(gb, "wCurMap"), 13);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 9);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 49);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wEnemyMonSpecies"), 3);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wEnemyMonSpecies"), 3);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wEnemyMonLevel"), 6);
    ASSERT_EQUALS(gameboy_cpuread16besym(gb, "wEnemyMonDVs"), 0xfaee);
    ASSERT_EQUALS(gameboy_cpuread16besym(gb, "wEnemyMonDVs"), 0xfaee);
    ASSERT_EQUALS(gameboy_yoloball(gb), true);
    return true;
}

bool yellow_pidgey(GameBoy *gb, int path) {
    gameboy_executeintro(gb, "gfskip_intro_title_cont_cont");
    ASSERT_EQUALS(gameboy_executepath(gb,"UUUUUULLLLLLLLLLLLLALDDDDDADDLLLAUUUUULAUUUUUUUULLLLADDDDDDDDDDDDLDDDDADDDLLLLLLUUAU"),
                  symbol_lookup(gb, "CalcStats"));
    ASSERT_EQUALS(gameboy_cpuread(gb, "wCurMap"), 51);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 1);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 19);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wEnemyMonSpecies"), 36);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wEnemyMonLevel"), 4);
    ASSERT_EQUALS(gameboy_yoloball(gb), true);
    return true;
}

bool yellow_moon(GameBoy *gb, int path) {
    gameboy_executeintro(gb, "gfskip_intro0_title0_cont_cont");
    ASSERT_EQUALS(gameboy_executepath(gb,"UAUUUUUUUUAUUURRRARRRURUUUUUURARRDDDDDDDDDDDDRDDDDDRRRRRRURRR"),
                  symbol_lookup(gb, "JoypadOverworld"));
    ASSERT_EQUALS(gameboy_cpuread(gb, "wCurMap"), 59);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 34);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 31);
    gameboy_pickupitem(gb);
    ASSERT_EQUALS(gameboy_executepath(gb,"RAUUUAUUUUUUULUUAUUUUUUUUAUULLUUUUULULLLLLLLDDDLLLLDLLLDDLDDDADDDDDLLLLLLLALLLUULULLUUUUUUULAUUUU"),
                  symbol_lookup(gb, "JoypadOverworld"));
    ASSERT_EQUALS(gameboy_cpuread(gb, "wCurMap"), 59);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 2);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 3);
    gameboy_pickupitem(gb);
    ASSERT_EQUALS(gameboy_executepath(gb,"DRRRD"),
                  symbol_lookup(gb, "JoypadOverworld"));
    ASSERT_EQUALS(gameboy_executepath(gb,"DDDDDDDDRDDDDRRRARRRARRRARRRRRR"),
                  symbol_lookup(gb, "JoypadOverworld"));
    ASSERT_EQUALS(gameboy_executepath(gb,"RRUUURRRDDARRRRARRUAURARRARDADDDADDDDADDLLDDADDDADDALLLLLALLLALLLLLLLLLLLLLLLUUUUUAUUUUAUUAUUUURUUAUUAUUU"),
                  symbol_lookup(gb, "JoypadOverworld"));
    return true;
}

#endif //MANIPS_YELLOW_H
