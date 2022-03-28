#ifndef MANIPS_GOLD_H
#define MANIPS_GOLD_H

bool gold_totodile(GameBoy *gb, int path) {
    int dvs[] = { 0xfdff, 0xdecf };
    
    gameboy_setrtc(gb, 1*60);
    gameboy_executeintro(gb, gsc_intro(11 + path));
    gameboy_cpuwrite(gb, "wGameTimeFrames", 23);
    ASSERT_EQUALS(gameboy_executepath(gb, "RS_BDA+RRDU"),
                  OVERWORLD_LOOP);
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

bool gold_r29(GameBoy *gb, int path) {
    const char *paths[] = {
        "LLLLLA+LDDLLDLLDLLDDLDLLLLLLLLLLULLUUURRUA+RRRUUUULULLLLLLLLLLLLLLUULLDLLA+DDLLLLA+LLLLA+LLLLLLLLLLLLLLLLLUULLLLULLLLLLLA+LLLUUUUA+UUUUUURRRRRRUUUUUUUUUUUUUUUUUURRUUUUUUUUUURRUURUUUULUUU",
        "LLLLLA+LDLLLLLLDDDDA+DLDLLLLLLLLLLULLUUUUURRRRRUUUULLLLLLLLLLLLLLLUULLLLLDDA+DLLLLLLLA+LLLLLLLLLLLLLLLLLUULLLLUA+LLLLLLLA+LLLUUUUUUUUURRRRRRUUUUUUUUUUUUUUUUUUURRUUUUUUUUUURRUURUUUULUUU",
        "LLLLLA+LDDLLLDDLDDLA+LLLLLLLDLLLLLUULLUUUURRRRRUUUULLLLLLLLLLLLLLLUULLLDLLA+DDLLLA+LLLLLLLLLLLLA+LLLLLLLLLULLLUULLLLLLLLLLLUUUUUUUUURRRRRRUUUUUUUUUUUURRUUUUUUUUUUUUUUUUURUURRUUUULUUU",
        "LLLLLA+LDDLLDLLDLLDDLDLLLLLLLLLLULLUUURRUA+RRRUUUULULLLLLLLLLLLLLLUULLDLLA+DDLLLLA+LLLLA+LLLLLLLLLLLLLLLLLULA+LULLUA+LLA+LLLLLLLLUUUUUUUUURRRRRUUUUUUUUUUUUUUA+UUULLUULLA+LUUUULLA+U",
        "LLLLLA+LDLLLLLLDDDDA+DLDLLLLLLLLLLULLUUUUURRRRRUUUULLLLLLLLLLLLLLLUULLLLLDDA+DLLLLLLLA+LLLLLLLLLLLLLLLLLLLUUULA+LLLLLLA+LLLLA+LUUUUA+UUUUUURRRA+RRUUUUUUUUUUUUUUULLLLUUULUULLUA+UU",
        "LLLLLA+LDDLLLDDLDDLA+LLLLLLLDLLLLLUULLUUUURRRRRUUUULLLLLLLLLLLLLLLUULLLDLLA+DDLLLA+LLLLLLLLLLLA+LLLLLLLLLLUUULLLLA+LLLLLLLLLLUUUUUUUUURURRRRUUUUUUUUUUUUUUUUUULLLLULUUA+LUA+LU",
    };
    
    gameboy_setrtc(gb, 10*60);
    gameboy_executeintro(gb, gsc_intro(14 + (path % 3)));
    ASSERT_EQUALS(gameboy_executepath(gb, paths[path]),
                  OVERWORLD_LOOP);
    
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

bool gold_don(GameBoy *gb, int path) {
    const char *paths[] = {
        "LUUUUUUULUUUUA+URUUURURA+RRUUA+UURUUUUUUA+RRRRRUA+UUUULLLA+LLLLLLLLDLLLLDA+LLLLULLLLUU",
        "LUUUUUUULUUUUA+URUUURURA+RUURUUURUS_BUUURRRRRUUUUUULLLLLA+LLLLLLDLLLLLDA+LLA+LLUULLLU",
        "LUUUUUUULUUUUA+URUUURURA+RUURUUUS_BRUUUURURRRRUA+UUUULLLLLLLLLLLDLLLLLDLLA+LLULUA+LLU",
        "LUUUUUUULUUUUA+URUUURURA+RRUURUUUS_BUUUUURRRRRUA+UUUULLLLLLLLLLLDLLLLLDLLLLLLLUUU",
    };
    
    gameboy_setrtc(gb, 15*60);
    gameboy_executeintro(gb, gsc_intro(12 + path));
    ASSERT_EQUALS(gameboy_executepath(gb, paths[path]),
                  OVERWORLD_LOOP);
    
    ASSERT_EQUALS(gameboy_cpuread16besym(gb, "wMapGroup"), 6658);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wXCoord"), 9);
    ASSERT_EQUALS(gameboy_cpuread(gb, "wYCoord"), 9);
    return true;
}

#endif //MANIPS_GOLD_H
