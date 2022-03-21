#ifndef MANIPS_H
#define MANIPS_H

#include <vector>
#include <string>
#include <stdio.h>

#ifndef ASSERT_EQUALS
#define ASSERT_EQUALS(got, expected) got;
#endif

typedef bool (*ManipProcedure)(GameBoy *, int);

struct Manip {
    const char *name;
    char *game;
    const char *rom;
    const char *sav;
    ManipProcedure procedure;
    int path;
    
    Manip(const char *name, const char *sav, ManipProcedure procedure, int path = 0) {
        game = strdup(name);
        copy_string_until(game, name, '-');
        
        char buffer[128];
        sprintf(buffer, "test\\roms\\poke%s.gbc", game);
        this->rom = strdup(buffer);
        sprintf(buffer, "test\\basesaves\\%s", sav);
        this->sav = strdup(buffer);
        
        this->name = name;
        this->procedure = procedure;
        this->path = path;
    }
};

char intro_buffer[64];
char * gsc_intro(int delay) {
    sprintf(intro_buffer, "gfskip_title_cont_delay%d_cont", delay);
    return intro_buffer;
}

#include "manips-red.h"
#include "manips-yellow.h"
#include "manips-gold.h"
#include "manips-crystal.h"

Manip all_manips[] = {
    Manip("red-triple-extended", "red_nido.sav", &red_triple_extended),
    Manip("red-route3-moon", "red_moon_route3.sav", &red_moon_route3),
    Manip("red-cans", "red_cans.sav", &red_cans),
    
    Manip("yellow-nido", "yellow_nido.sav", &yellow_nido),
    Manip("yellow-pidgey", "yellow_pidgey.sav", &yellow_pidgey),
    Manip("yellow-moon", "yellow_moon_no_mp.sav", &yellow_moon),
    
    Manip("gold-totodile-frame1", "gold_toto_4_3.sav", &gold_totodile, 0),
    Manip("gold-totodile-frame2", "gold_toto_4_3.sav", &gold_totodile, 1),
    Manip("gold-r29-pass1-path1", "gold_r29_pass1.sav", &gold_r29, 0),
    Manip("gold-r29-pass1-path2", "gold_r29_pass1.sav", &gold_r29, 1),
    Manip("gold-r29-pass1-path3", "gold_r29_pass1.sav", &gold_r29, 2),
    Manip("gold-r29-pass2-path1", "gold_r29_pass2.sav", &gold_r29, 3),
    Manip("gold-r29-pass2-path2", "gold_r29_pass2.sav", &gold_r29, 4),
    Manip("gold-r29-pass2-path3", "gold_r29_pass2.sav", &gold_r29, 5),
    Manip("gold-don-path1", "gold_donnies.sav", &gold_don, 0),
    Manip("gold-don-path2", "gold_donnies.sav", &gold_don, 1),
    Manip("gold-don-path3", "gold_donnies.sav", &gold_don, 2),
    
    Manip("crystal-totodile-frame1", "crystal_toto_4_4.sav", &crystal_totodile, 0),
    Manip("crystal-totodile-frame2", "crystal_toto_4_4.sav", &crystal_totodile, 1),
    Manip("crystal-r29-pass1-path1", "crystal_r29_pass1.sav", &crystal_r29, 0),
    Manip("crystal-r29-pass1-path2", "crystal_r29_pass1.sav", &crystal_r29, 1),
    Manip("crystal-r29-pass1-path3", "crystal_r29_pass1.sav", &crystal_r29, 2),
    Manip("crystal-r29-pass2-path1", "crystal_r29_pass2.sav", &crystal_r29, 3),
    Manip("crystal-r29-pass2-path2", "crystal_r29_pass2.sav", &crystal_r29, 4),
    Manip("crystal-r29-pass2-path3", "crystal_r29_pass2.sav", &crystal_r29, 5),
    Manip("crystal-r30.5-path-1", "crystal_r30_5.sav", &crystal_r30_5, 0),
    Manip("crystal-r30.5-path-2", "crystal_r30_5.sav", &crystal_r30_5, 1),
    Manip("crystal-r30.5-path-3", "crystal_r30_5.sav", &crystal_r30_5, 2),
    Manip("crystal-r32-path-1", "crystal_r32.sav", &crystal_r32, 0),
    Manip("crystal-r32-path-2", "crystal_r32.sav", &crystal_r32, 1),
    Manip("crystal-r32-path-3", "crystal_r32.sav", &crystal_r32, 2),
    Manip("crystal-raikou-path-1", "crystal_raikou.sav", &crystal_raikou, 0),
    Manip("crystal-raikou-path-2", "crystal_raikou.sav", &crystal_raikou, 1),
    Manip("crystal-raikou-path-3", "crystal_raikou.sav", &crystal_raikou, 2),
};

#endif //MANIPS_H