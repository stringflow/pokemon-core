#ifndef MANIPS_CRYSTAL_H
#define MANIPS_CRYSTAL_H

bool crystal_totodile(GameBoy *gb, int path) {
    const char *paths[] = { "DA+RRA+RUS_B", "DA+RRA+RU" };
    int dvs[] = { 0xffff, 0xffef };
    
    gameboy_setrtc(gb, 1*60);
    gameboy_executeintro(gb, gsc_intro(61 + path));
    gameboy_cpuwrite(gb, "wGameTimeFrames", 4);
    ASSERT_EQUALS(gameboy_executepath(gb, paths[path]),
                  symbol_lookup(gb, "OWPlayerInput"));
    ASSERT_EQUALS(gameboy_cpuread16besym(gb, "wMapGroup"), 6149);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 7);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 4);
    gameboy_press(gb, A);
    gameboy_press(gb, B);
    gameboy_cleartext(gb, B);
    gameboy_press(gb, A);
    gameboy_cleartext(gb, B);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wPartyMon1Species"), 158);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wPartyMon1Level"), 5);
    ASSERT_EQUALS(gameboy_cpuread16besym(gb, "wPartyMon1DVs"), dvs[path]);
    return true;
}

bool crystal_r29(GameBoy *gb, int path) {
    const char *paths[] = {
        "LLLLLLLLLLDDDLDDLLDDLLLLLLLLLLUULUUUURRRRRUUULULLA+LLLULLLLLLLLLULLDLLA+LDLA+DLA+LLLLLLLLLLLLLLLLLLLLLLLUUULLLLLLLLLLLLLUUUUUUUUURRRRRUUUUURUUUUUUURRUUUUUUUUUUUUUUUUURUURRUUUULUUU",
        "LLLLLLLLLLDDDLDDLLDDLLLLLLLLLLUULUUUURRRRRUUULULLLLLULA+LLLLLLLLULLDLLA+LDA+LDA+LLLLLLLLLLLLLLLLLLLLLLLLLLA+UUULLLLLLLLLLLUUUUUUUUURRRRRUUUUURUUUUUUUUUUURUURUUUUUUUUUUURUURRUUUULUUU",
        "LLLLLLLLLLDDDLDDLLDDLLLLLLLLLUULLUUUURRRRRUUULLLULLLULLLLLLLLLULLLLA+LDDLLLA+LLLLA+LLLLLLLLLLLLLLLDLA+LLLLUUULLLLLLLLLLLUUUUUUUUURRRRRUUUUURUUUUUUUUUURURUUUUUUUUUUUUURRUURUUUULUUU",
        "LLLLLLLLLLDDDLDDLLDDLLLLLLLLLLUULUUUURRRRRUUULULLA+LLLULLLLLLLLLULLDLLA+LDLA+DLA+LLLLLLLLLLLLLLLLLLLLLLLUA+UULLLLLLLLLLLLLUUUUUUUUURURRRRUUUUUUUUUUUUUUULLUULLLLUUUULUU",
        "LLLLLLLLLLDDDLDDLLDDLLLLLLLLLLUULUUUURRRRRUUULULLLLLULA+LLLLLLLLULLDLLA+LDA+LDA+LLLLLLLLLLLLLLLLLLLLLLA+LLLLLA+LUA+UULLLLLLLLLUUUUUUUUUURRRRRUUUUUUUUUUUUUA+UUUUULLLUULLULLUU",
        "LLLLLLLLLLDDDLDDLLDDLLLLLLLLLUULLUUUURRRRRUUULLLULLLULLLLLLLLLULLLLA+LDDLLLA+LLLLLLLLLLLLLLLLLLLDLLLLUULA+ULLLA+LLA+LLLLLA+LUUUUUUUUUURRRRRUUUUUUUUUUUUUUA+LUUUULLA+LUULULLUU"
    };
    
    gameboy_setrtc(gb, 10*60);
    gameboy_executeintro(gb, gsc_intro(2 + (path % 3)));
    ASSERT_EQUALS(gameboy_executepath(gb, paths[path]),
                  symbol_lookup(gb, "OWPlayerInput"));
    
    if(path < 3) {
        ASSERT_EQUALS(gameboy_cpuread16besym(gb, "wMapGroup"), 6657);
        ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 17);
        ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 11);
    } else {
        ASSERT_EQUALS(gameboy_cpuread16besym(gb, "wMapGroup"), 6657);
        ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 5);
        ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 25);
    }
    
    return true;
}

bool crystal_r30_5(GameBoy *gb, int path) {
    gameboy_setrtc(gb, 15*60);
    gameboy_executeintro(gb, gsc_intro(path));
    ASSERT_EQUALS(gameboy_executepath(gb,"UUUUUUUULULUUUUUUURRRRRRUUUUUUUUURRRRRRUUUUUULLLLLLLLLLLDDLLLA+LLLLLUA+LLLUA+LU"),
                  symbol_lookup(gb, "OWPlayerInput"));
    ASSERT_EQUALS(gameboy_cpuread16besym(gb, "wMapGroup"), 6658);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 9);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 9);
    return true;
}

bool crystal_r32(GameBoy *gb, int path) {
    gameboy_setrtc(gb, 20*60);
    gameboy_executeintro(gb, gsc_intro(path));
    ASSERT_EQUALS(gameboy_executepath(gb,"DDDDDDDDDDLLDDDLLLLLLDDDLLDLLLDLDDLD"),
                  symbol_lookup(gb, "OWPlayerInput"));
    ASSERT_EQUALS(gameboy_cpuread16besym(gb, "wMapGroup"), 2561);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 3);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 29);
    gameboy_pickupitem(gb);
    ASSERT_EQUALS(gameboy_executepath(gb,"DDDDDLDDDDDDDDLDDDDDDDA+DDDDDDDDDDRDDRRA+RDDDDDD"),
                  symbol_lookup(gb, "OWPlayerInput"));
    ASSERT_EQUALS(gameboy_cpuread16besym(gb, "wMapGroup"), 2561);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 5);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 67);
    return true;
}


bool crystal_raikou(GameBoy *gb, int path) {
    const char *paths[] = {
        "RRRSELRDDDDDDUA+DRLRDDDDDLS_BURRUUDUUDA+D",
        "RRRSELRDDDDDDUA+DRLRDDA+DDLUS_BRS_BRUD",
        "RRRSELRDDDDDDUA+DRLRDDDDS_BDUULLLSELR"
    };
    
    int end_coordinates[] = {
        9, 2,
        9, 2,
        6, 2
    };
    
    int dvs[] = {
        0xfd9e,
        0xfdbf,
        0xfdbf
    };
    
    gameboy_setrtc(gb, 40*60);
    gameboy_executeintro(gb, gsc_intro(11+path));
    ASSERT_EQUALS(gameboy_executepath(gb, paths[path]),
                  symbol_lookup(gb, "RandomEncounter.ok"));
    gameboy_rununtil(gb, "CalcMonStats", NONE);
    ASSERT_EQUALS(gameboy_cpuread16besym(gb, "wMapGroup"), 2564);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), end_coordinates[path*2+0]);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), end_coordinates[path*2+1]);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wEnemyMonSpecies"), 243);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wEnemyMonLevel"), 40);
    ASSERT_EQUALS(gameboy_cpuread16besym(gb, "wEnemyMonDVs"), dvs[path]);
    return true;
}

#endif //MANIPS_CRYSTAL_H
