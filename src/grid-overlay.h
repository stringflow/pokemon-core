#ifndef GRID_OVERLAY_H
#define GRID_OVERLAY_H

#include <emmintrin.h>

struct BGMapScroll {
    u8 x;
    u8 y;
};

struct Grid {
    s16 min_x;
    s16 min_y;
    s16 max_x;
    s16 max_y;
    s16 tile_x;
    s16 tile_y;
    std::map<int, int> tile_colors;
    int previous_action_hash;
    
    int width;
    int height;
    int *pixels;
    
    BGMapScroll scroll;
    int viewport_x;
    int viewport_y;
    
    bool *hidden_pixels;
};

struct OAMSprite {
    u8 y;
    u8 x;
    u8 tile;
    u8 flags;
};

Grid grid_create(GameBoy *gb, const char *path);
void grid_free(Grid *grid);
void grid_update_viewport(GameBoy *gb, Grid *grid);
void grid_blittovideo(GameBoy *gb, int *dest, Grid *src);

#endif //GRID_OVERLAY_H
