#include "..\src\gameboy.cpp"
#include <vector>
#include <map>
#include <algorithm>

static int global_got;
static int global_expected;

static char buffer[256];
static int total_asserts;
static std::vector<std::string> failures;

#define ASSERT_EQUALS(got, expected) \
total_asserts++;\
global_got = (got); \
global_expected = (expected); \
if(global_got != global_expected) { \
sprintf(buffer, "%s:%d: ASSERT_EQUALS: Expected %04x but got %04x.", __FILE__, __LINE__, global_expected, global_got); \
failures.push_back(std::string(buffer)); \
return false; \
}

#include "manips.h"

struct TestGame {
    std::vector<Manip> manips;
    int procedures_ran;
    int procedures_successful;
    bool failure = false;
};

int main(int argc, char *argv[]) {
    std::map<std::string, TestGame> test_games;
    std::vector<std::string> game_names;
    for(Manip manip : all_manips) {
        std::string name(manip.game);
        
        test_games[name].manips.push_back(manip);
        
        // NOTE(stringflow): KEEP TRACK OF INSERTION ORDER BECAUSE APPARENTLY IT'S
        // IMPOSSIBLE WITH JUST USING MAPS
        if(std::find(game_names.begin(), game_names.end(), name) == game_names.end()) {
            game_names.push_back(name);
        }
    }
    
    for(std::string game_name : game_names) {
        TestGame& game = test_games[game_name];
        printf("poke%s\n", game_name.c_str());
        
        for(Manip& manip : game.manips) {
            printf("\t%s ... ", manip.name);
            fflush(stdout);
            
            GameBoy *gb = gameboy_create();
            gameboy_load(gb, "test\\roms\\gbc_bios.bin", manip.rom, manip.sav);
            gameboy_setspeedupflags(gb, gb->gamedata.max_speedup_flags);
            
            bool result = manip.procedure(gb, manip.path);
            game.procedures_ran++;
            if(result) {
                printf("OK\n");
                game.procedures_successful++;
            } else {
                printf("FAILED\n");
            }
            
            fflush(stdout);
            gameboy_free(gb);
        }
        
        r32 percent = (r32) game.procedures_successful / (r32) game.procedures_ran * 100.0f;
        printf("%.1f%% success (%d out of %d)\n\n", percent, game.procedures_successful, game.procedures_ran);
        fflush(stdout);
    }
    
    
    int games_ran = game_names.size();
    int games_successful = 0;
    int total_procedures = 0;
    for(auto pair : test_games) {
        TestGame game = pair.second;
        
        total_procedures += game.procedures_ran;
        if(game.procedures_successful == game.procedures_ran) {
            games_successful++;
        }
    }
    
    r32 percent = (r32) games_successful / (r32) games_ran * 100.0f;
    printf("[Test Suite] %.1f%% success (%d out of %d)\n", percent, games_successful, games_ran);
    
    if(games_successful == games_ran) {
        printf("\t*** %d test games\n", games_ran);
        printf("\t*** %d test procedures\n", total_procedures);
        printf("\t*** %d test asserts\n", total_asserts);
    } else {
        for(std::string failure : failures) {
            printf("\t>>> FAILED: %s\n", failure.c_str());
        }
    }
}