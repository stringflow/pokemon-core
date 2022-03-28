#ifndef MANIPS_RED_H
#define MANIPS_RED_H

bool red_triple_extended(GameBoy *gb, int path) {
    gameboy_executeintro(gb, "nopal_gfskip_hop0_title0_cont_cont");
    ASSERT_EQUALS(gameboy_executepath(gb,"LLLULLUAULALDLDLLDADDADLALLALUUAU"),
                  WILD_ENCOUNTER);
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
                  WILD_ENCOUNTER);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wCurMap"), 13);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 8);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 48);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wEnemyMonSpecies"), 36);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wEnemyMonLevel"), 5);
    ASSERT_EQUALS(gameboy_yoloball(gb), true);
    gameboy_cleartext(gb, A);
    gameboy_press(gb, B);
    
    ASSERT_EQUALS(gameboy_executepath(gb,"UUUULLLLLU"),
                  OVERWORLD_LOOP);
    ASSERT_EQUALS(gameboy_executepath(gb,"UUUUURUU"),
                  OVERWORLD_LOOP);
    ASSERT_EQUALS(gameboy_executepath(gb,"UUAUURUARRRRRRRUUUUUUUUUUUAUUAUUUUUUUUUUUUUUUUU"),
                  OVERWORLD_LOOP);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wCurMap"), 51);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 25);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 12);
    gameboy_pickupitem(gb);
    
    ASSERT_EQUALS(gameboy_executepath(gb,"UUULLLLLLLLDDDDDDDLLLLUUUUUUUUUUUUULLLLLLDDDDDDDDDDDDDDDDDDLDLLLLUUU"),
                  OVERWORLD_LOOP);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wCurMap"), 51);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 2);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 19);
    return true;
}


bool red_moon_route3(GameBoy *gb, int path) {
    gameboy_executeintro(gb, "pal(hold)_gfskip_hop0_title0_cont_cont");
    ASSERT_EQUALS(gameboy_executepath(gb,"RRRRRRRRURRUUUUUARRRRRRRRRRRRDDDDDRRRRRRRARUURRUUUUUUUUUURRRRUUUUUUUUUURRRRRU"),
                  OVERWORLD_LOOP);
    ASSERT_EQUALS(gameboy_executepath(gb,"UUUUUULLLLLALLLLDD"),
                  OVERWORLD_LOOP);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wCurMap"), 59);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 5);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 31);
    gameboy_pickupitem(gb);
    
    ASSERT_EQUALS(gameboy_executepath(gb,"RRRRUURRRARRUUUUUUURRRRRRRAUUUUUUURRRDRDDDDDDDADDDDDDDDADRRRRRURRRR"),
                  OVERWORLD_LOOP);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wCurMap"), 59);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 34);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 31);
    gameboy_pickupitem(gb);
    
    ASSERT_EQUALS(gameboy_executepath(gb,"UUUUUUUUR"),
                  OVERWORLD_LOOP);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wCurMap"), 59);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 35);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 23);
    gameboy_pickupitem(gb);
    
    ASSERT_EQUALS(gameboy_executepath(gb,"ULUUUUUAUUUUUULLLUUUUUUUULLLLLLDDLALLLLLLLDDDDDD"),
                  OVERWORLD_LOOP);
    ASSERT_EQUALS(gameboy_executepath(gb,"LALLALLALLALDD"),
                  OVERWORLD_LOOP);
    ASSERT_EQUALS(gameboy_executepath(gb,"RRRUUULAUR"),
                  OVERWORLD_LOOP);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wCurMap"), 61);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 28);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 5);
    gameboy_pickupitem(gb);
    
    ASSERT_EQUALS(gameboy_executepath(gb,"DDADLALLAD"),
                  OVERWORLD_LOOP);
    ASSERT_EQUALS(gameboy_executepath(gb,"RARRARRARRARUU"),
                  OVERWORLD_LOOP);
    ASSERT_EQUALS(gameboy_executepath(gb,"DDLDDDDLLLLLLLULUUUUULUUUUUUUULLLUL"),
                  OVERWORLD_LOOP);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wCurMap"), 59);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 3);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 2);
    gameboy_pickupitem(gb);
    
    ASSERT_EQUALS(gameboy_executepath(gb,"DADDRAR"),
                  OVERWORLD_LOOP);
    ASSERT_EQUALS(gameboy_executepath(gb,"DRRDDDDDDDDDDRRRARRRRRRRRRRDR"),
                  OVERWORLD_LOOP);
    ASSERT_EQUALS(gameboy_executepath(gb,"RRUUURARRRDDRRRRRUARURARRDDDDDDDDALLLLDDDDDDDADDLLLALLLLLLLLLLLLALLLLLLUUUUAUUALUUUUUUUU"),
                  WILD_ENCOUNTER);
    
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
                  OVERWORLD_LOOP);
    ASSERT_EQUALS(gameboy_executepath(gb,"RUUUUU"),
                  OVERWORLD_LOOP);
    gameboy_press(gb, A);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wCurMap"), 92);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 5);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 12);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wFirstLockTrashCanIndex"), 8);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wSecondLockTrashCanIndex"), 5);
    return true;
}

#endif //MANIPS_RED_H
