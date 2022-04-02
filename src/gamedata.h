#ifndef GAMEDATA_H
#define GAMEDATA_H

struct GameData {
    int joypad_address;
    int battle_start_address;
    int xcoord_address;
    int ycoord_address;
    int map_address;
    int rom_bank_address;
    u8 grid_line_shade;
    int max_speedup_flags;
};

const GameData RED_DATA = { 
    .joypad_address = symbol_lookup_red("_Joypad"),
    .battle_start_address = symbol_lookup_red("SlidePlayerAndEnemySilhouettesOnScreen"),
    .xcoord_address = symbol_lookup_red("wXCoord"),
    .ycoord_address = symbol_lookup_red("wYCoord"),
    .map_address = symbol_lookup_red("wCurMap"),
    .rom_bank_address = symbol_lookup_red("hLoadedROMBank"),
    .grid_line_shade = 0x00,
    .max_speedup_flags = NO_VIDEO | NO_SOUND | NO_PPU_CALL,
};

const GameData BLUE_DATA = { 
    .joypad_address = symbol_lookup_blue("_Joypad"),
    .battle_start_address = symbol_lookup_blue("SlidePlayerAndEnemySilhouettesOnScreen"),
    .xcoord_address = symbol_lookup_blue("wXCoord"),
    .ycoord_address = symbol_lookup_blue("wYCoord"),
    .map_address = symbol_lookup_blue("wCurMap"),
    .rom_bank_address = symbol_lookup_blue("hLoadedROMBank"),
    .grid_line_shade = 0x00,
    .max_speedup_flags = NO_VIDEO | NO_SOUND | NO_PPU_CALL,
};

const GameData YELLOW_DATA = { 
    .joypad_address = symbol_lookup_yellow("_Joypad"),
    .battle_start_address = symbol_lookup_yellow("SlidePlayerAndEnemySilhouettesOnScreen"),
    .xcoord_address = symbol_lookup_yellow("wXCoord"),
    .ycoord_address = symbol_lookup_yellow("wYCoord"),
    .map_address = symbol_lookup_yellow("wCurMap"),
    .rom_bank_address = symbol_lookup_yellow("hLoadedROMBank"),
    .grid_line_shade = 0x18,
    .max_speedup_flags = NO_VIDEO | NO_SOUND,
};

const GameData GOLD_DATA = { 
    .joypad_address = symbol_lookup_gold("GetJoypad.quit")-2,
    .battle_start_address = symbol_lookup_gold("DoBattleTransition.done"),
    .xcoord_address = symbol_lookup_gold("wXCoord"),
    .ycoord_address = symbol_lookup_gold("wYCoord"),
    .map_address = symbol_lookup_gold("wMapNumber"),
    .rom_bank_address = symbol_lookup_gold("hROMBank"),
    .grid_line_shade = 0x38,
    .max_speedup_flags = NO_VIDEO | NO_SOUND,
};

const GameData SILVER_DATA = { 
    .joypad_address = symbol_lookup_silver("GetJoypad.quit")-2,
    .battle_start_address = symbol_lookup_silver("DoBattleTransition.done"),
    .xcoord_address = symbol_lookup_silver("wXCoord"),
    .ycoord_address = symbol_lookup_silver("wYCoord"),
    .map_address = symbol_lookup_silver("wMapNumber"),
    .rom_bank_address = symbol_lookup_silver("hROMBank"),
    .grid_line_shade = 0x38,
    .max_speedup_flags = NO_VIDEO | NO_SOUND,
};

const GameData CRYSTAL_DATA = { 
    .joypad_address = symbol_lookup_crystal("GetJoypad.quit")-2,
    .battle_start_address = symbol_lookup_crystal("DoBattleTransition.done"),
    .xcoord_address = symbol_lookup_crystal("wXCoord"),
    .ycoord_address = symbol_lookup_crystal("wYCoord"),
    .map_address = symbol_lookup_crystal("wMapNumber"),
    .rom_bank_address = symbol_lookup_crystal("hROMBank"),
    .grid_line_shade = 0x38,
    .max_speedup_flags = NO_VIDEO | NO_SOUND,
};

#endif //GAMEDATA_H
