const __m128 n255_4 = _mm_set1_ps(255.0f);
const __m128 inv255_4 = _mm_div_ps(_mm_set1_ps(1.0f), n255_4);
const __m128i maskff_4 = _mm_set1_epi32(0xff);

// TODO(stringflow): write mask instead of the out buffer?
void alphablend(int *out, int *dest, int *src) {
    __m128i dest_pix = _mm_loadu_si128((__m128i *) dest);
    __m128i src_pix = _mm_loadu_si128((__m128i *) src);
    
    // NOTE(stringflow): src_a = (1.0f / 255.0f) * (r32) (src_pix >> 24)
    __m128 src_a = _mm_mul_ps(inv255_4, _mm_cvtepi32_ps(_mm_srli_epi32(src_pix, 24)));
    // NOTE(stringflow): src_r = (r32) ((src_pix >> 16) & 0xff)
    __m128 src_r = _mm_cvtepi32_ps(_mm_and_si128(_mm_srli_epi32(src_pix, 16), maskff_4));
    // NOTE(stringflow): src_g = (r32) ((src_pix >> 8) & 0xff)
    __m128 src_g = _mm_cvtepi32_ps(_mm_and_si128(_mm_srli_epi32(src_pix, 8), maskff_4));
    // NOTE(stringflow): src_b = (r32) (src_pix & 0xff)
    __m128 src_b = _mm_cvtepi32_ps(_mm_and_si128(src_pix, maskff_4));
    
    // NOTE(stringflow): dest_a = (1.0f / 255.0f) * (r32) (dest_pix >> 24)
    __m128 dest_a = _mm_mul_ps(inv255_4, _mm_cvtepi32_ps(_mm_srli_epi32(dest_pix, 24)));
    // NOTE(stringflow): dest_r = (r32) ((dest_pix >> 16) & 0xff)
    __m128 dest_r = _mm_cvtepi32_ps(_mm_and_si128(_mm_srli_epi32(dest_pix, 16), maskff_4));
    // NOTE(stringflow): dest_g = (r32) ((dest_pix >> 8) & 0xff)
    __m128 dest_g = _mm_cvtepi32_ps(_mm_and_si128(_mm_srli_epi32(dest_pix, 8), maskff_4));
    // NOTE(stringflow): dest_b = (r32) (dest_pix & 0xff)
    __m128 dest_b = _mm_cvtepi32_ps(_mm_and_si128(dest_pix, maskff_4));
    
    // NOTE(stringflow): blended_a = 255.0f * (src_a + dest_a - src_a * dest_a)
    __m128 blended_a = _mm_mul_ps(n255_4, _mm_sub_ps(_mm_add_ps(src_a, dest_a), 
                                                     _mm_mul_ps(src_a, dest_a)));
    // NOTE(stringflow): blended_r = dest_r + src_a * (src_r - dest_r)
    __m128 blended_r = _mm_add_ps(dest_r, _mm_mul_ps(src_a, _mm_sub_ps(src_r, dest_r)));
    // NOTE(stringflow): blended_g = dest_g + src_a * (src_g - dest_g)
    __m128 blended_g = _mm_add_ps(dest_g, _mm_mul_ps(src_a, _mm_sub_ps(src_g, dest_g)));
    // NOTE(stringflow): blended_b = dest_b + src_a * (src_b - dest_b)
    __m128 blended_b = _mm_add_ps(dest_b, _mm_mul_ps(src_a, _mm_sub_ps(src_b, dest_b)));
    
    // NOTE(stringflow): out_a = (int) (blended_a) << 24
    __m128i out_a = _mm_slli_epi32(_mm_cvtps_epi32(blended_a), 24);
    // NOTE(stringflow): out_r = (int) (blended_r) << 16
    __m128i out_r = _mm_slli_epi32(_mm_cvtps_epi32(blended_r), 16);
    // NOTE(stringflow): out_g = (int) (blended_g) << 8
    __m128i out_g = _mm_slli_epi32(_mm_cvtps_epi32(blended_g), 8);
    // NOTE(stringflow): out_b = (int) (blended_b)
    __m128i out_b = _mm_cvtps_epi32(blended_b);
    
    // NOTE(stringflow): out_pix = (out_a | out_r) | (out_g | out_b)
    __m128i out_pix = _mm_or_si128(_mm_or_si128(out_a, out_r), _mm_or_si128(out_g, out_b));
    
    _mm_storeu_si128((__m128i *) out, out_pix);
}

void grid_fillrect(Grid *dest, int x, int y, int width, int height, int color) {
    int color4[] = { color, color, color, color };
    int pixel_buffer[4];
    
    int *dest_row = dest->pixels + x + y * dest->width;
    for(int yy = 0; yy < height; yy++) {
        int *dest_ptr = dest_row;
        for(int xx = 0; xx < width;) {
            alphablend(pixel_buffer, dest_ptr, color4);
            
            for(int i = 0; i < 4 && xx < width; i++) {
                *dest_ptr++ = pixel_buffer[i];
                xx++;
            }
        }
        dest_row += dest->width;
    }
}

void grid_color_tile(Grid *grid, s16 x, s16 y, int color) {
    grid->tile_colors[(x << 16) | (y & 0xffff)] = color;
    grid->min_x = MIN(grid->min_x, x);
    grid->min_y = MIN(grid->min_y, y);
    grid->max_x = MAX(grid->max_x, x);
    grid->max_y = MAX(grid->max_y, y);
}

constexpr int MOVE_COLOR    = 0xb0ffff00;
constexpr int A_COLOR       = 0xb000ff00;
constexpr int S_B_COLOR     = 0xb00000ff;
constexpr int S_B_S_B_COLOR = 0xb0ff00ff;
constexpr int SEL_COLOR     = 0xb0ff0000;

void grid_walk_tile(Grid *grid, char *action) {
    int hashcode = hash(action);
    
    switch(hashcode) {
        case hash("A+L"): grid_color_tile(grid, grid->tile_x, grid->tile_y, A_COLOR);
        case hash("L"): {
            grid->tile_x--;
        } break;
        case hash("A+R"): grid_color_tile(grid, grid->tile_x, grid->tile_y, A_COLOR);
        case hash("R"): {
            grid->tile_x++;
        } break;
        case hash("A+U"): grid_color_tile(grid, grid->tile_x, grid->tile_y, A_COLOR);
        case hash("U"): {
            grid->tile_y--;
        } break;
        case hash("A+D"): grid_color_tile(grid, grid->tile_x, grid->tile_y, A_COLOR);
        case hash("D"): {
            grid->tile_y++;
        } break;
    }
    
    int color = MOVE_COLOR;
    
    switch(hashcode) {
        case hash("A"): {
            color = A_COLOR;
        } break;
        case hash("S_B"): {
            color = S_B_COLOR;
            if(grid->previous_action_hash == hashcode) color = S_B_S_B_COLOR;
        } break;
        case hash("SEL"): {
            color = SEL_COLOR;
        } break;
    }
    
    grid_color_tile(grid, grid->tile_x, grid->tile_y, color);
    grid->previous_action_hash = hashcode;
}

BGMapScroll read_scroll(GameBoy *gb) {
    BGMapScroll result = {
        .x = gameboy_cpuread(gb, 0xff43),
        .y = gameboy_cpuread(gb, 0xff42)
    };
    return result;
}

void grid_produce_image(GameBoy *gb, Grid *grid) {
    grid->width = (grid->max_x - grid->min_x) * 16 + VIDEO_WIDTH;
    grid->height = (grid->max_y - grid->min_y) * 16 + VIDEO_HEIGHT;
    grid->pixels = new int[grid->width * grid->height]();
    grid->scroll = read_scroll(gb);
    grid->viewport_x = -grid->min_x * 16;
    grid->viewport_y = -grid->min_y * 16;
    grid->hidden_pixels = new bool[VIDEO_SIZE];
    
    for(std::pair<int, int> element : grid->tile_colors) {
        // NOTE(stringflow): +4 to offset the path from the top left of the screen to 
        // where the player is drawn.
        s16 x = (s16) (element.first >> 16) - grid->min_x + 4;
        s16 y = (s16) (element.first & 0xffff) - grid->min_y + 4;
        int color = element.second;
        grid_fillrect(grid, x*16, y*16, 16, 16, color);
    }
    
    // TODO(stringflow): Color for grid lines should be read from the game's palette, but
    // documentation is awkward.
    int lines_color = ((0xff << 24) |
                       (gb->gamedata.grid_lines_shade << 16) | 
                       (gb->gamedata.grid_lines_shade <<  8) | 
                       (gb->gamedata.grid_lines_shade <<  0));
    
    for(int x = 0; x < grid->width; x += 16) {
        grid_fillrect(grid, x, 0, 1, grid->height, lines_color);
    }
    
    for(int y = 0; y < grid->height; y += 16) {
        grid_fillrect(grid, 0, y, grid->width, 1, lines_color);
    }
}

void draw_path_with_prerun(GameBoy *gb, Grid *grid, StringIterator *it) {
    int *original_video = gb->video_buffer;
    gb->video_buffer = gb->video_buffer_copy;
    
    int old_speedup_flags = gb->speedup_flags;
    int old_buffer_samples = gb->buffer_samples;
    gameboy_savestate(gb, gb->state_buffer);
    gameboy_setspeedupflags(gb, gb->gamedata.max_speedup_flags);
    gb->callbacks_disabled = true;
    
    // TODO(stringflow): check wMapGroup for gen 2 too
    u8 previous_x = gameboy_cpuread(gb, gb->gamedata.xcoord_address);
    u8 previous_y = gameboy_cpuread(gb, gb->gamedata.ycoord_address);
    u8 previous_map = gameboy_cpuread(gb, gb->gamedata.map_address);
    
    ExecutionResult result = OVERWORLD_LOOP;
    while(iterate_over_path(it) && result == OVERWORLD_LOOP) {
        result = gameboy_execute(gb, it->current_element);
        
        u8 new_x = gameboy_cpuread(gb, gb->gamedata.xcoord_address);
        u8 new_y = gameboy_cpuread(gb, gb->gamedata.ycoord_address);
        u8 new_map = gameboy_cpuread(gb, gb->gamedata.map_address);
        
        grid_walk_tile(grid, it->current_element);
        if(previous_map == new_map && 
           (ABS(new_x - previous_x) + (ABS(new_y - previous_y)) == 2)) {
            grid_walk_tile(grid, it->current_element);
        }
        
        previous_x = new_x;
        previous_y = new_y;
        previous_map = new_map;
    }
    
    gb->callbacks_disabled = false;
    gb->buffer_samples = old_buffer_samples;
    gameboy_loadstate(gb, gb->state_buffer, gb->state_size);
    gameboy_setspeedupflags(gb, old_speedup_flags);
    
    gb->video_buffer = original_video;
}

void draw_path_directly(Grid *grid, StringIterator *it) {
    while(iterate_over_path(it)) {
        grid_walk_tile(grid, it->current_element);
    }
}

bool should_prerun(GameBoy *gb) {
    if(gb->game & RBY) {
        // NOTE(stringflow): the top bit is used as a flag for... nothing?
        u8 tileset = gameboy_cpuread(gb, "wCurMapTileset") & ~0x80;
        return tileset == 0;
    } else {
        // TODO(stringflow): does gen2 do any more checks here?
        return true;
    }
}

Grid grid_create(GameBoy *gb, const char *path) {
    Grid grid = {};
    grid_color_tile(&grid, grid.tile_x, grid.tile_y, MOVE_COLOR);
    
    StringIterator it = { path };
    if(should_prerun(gb)) {
        draw_path_with_prerun(gb, &grid, &it);
    } else {
        draw_path_directly(&grid, &it);
    }
    
    grid_produce_image(gb, &grid);
    
    return grid;
}

void grid_free(Grid *grid) {
    if(grid->pixels) delete[] grid->pixels;
    if(grid->hidden_pixels) delete[] grid->hidden_pixels;
    grid->pixels = nullptr;
    grid->hidden_pixels = nullptr;
}

void grid_update_viewport(GameBoy *gb, Grid *grid) {
    BGMapScroll new_scroll = read_scroll(gb);
    
    int diff_x = diff8(grid->scroll.x, new_scroll.x);
    int diff_y = diff8(grid->scroll.y, new_scroll.y);
    
    // NOTE(stringflow): gen 1 battles annoyingly leave the scroll registers at 2,0
    // so I'm checking against the flag for when the battle ends to set the change to
    // zero, so the grid isn't desynced.
    // TODO(stringflow): check what happens here in gen 2?
    if(gb->game & RBY && gameboy_cpuread(gb, "wd72e") & 0x20) {
        diff_x = 0;
        diff_y = 0;
    }
    
    // NOTE(stringflow): LoadMapData clears the scroll registers to zero, which we don't
    // want to count as a "change". Not sure if there's a prettier way of achieving this,
    // I tried to check LCDC for if the LCD is disabled, but that isn't reliable
    // timing-wise.
    // TODO(stringflow): Maybe check map similar to the pre-run?
    constexpr int TRANSITION_THRESHOLD = 4;
    bool map_transition = new_scroll.y == 0 && new_scroll.x == 0 && (ABS(diff_y) > TRANSITION_THRESHOLD || ABS(diff_x) > TRANSITION_THRESHOLD);
    
    if(!map_transition) {
        grid->viewport_x += diff_x;
        grid->viewport_y += diff_y;
    }
    
    grid->scroll = new_scroll;
}

void grid_hide_behind_sprites(Grid *grid, u8 *oam, u8 *vram) {
    for(int sprite_index = 0; sprite_index < 40; sprite_index++) {
        OAMSprite *sprite = (OAMSprite *) oam;
        oam += sizeof(OAMSprite);
        
        bool mirror_y = sprite->flags & 0x40;
        bool mirror_x = sprite->flags & 0x20;
        int vram_bank = (sprite->flags >> 3) & 1;
        
        u8 *sprite_pixels = vram + sprite->tile * 16 + vram_bank * 0x2000;
        
        for(int y = 0; y < 8; y++) {
            int sprite_pixel_y = y;
            if(mirror_y) sprite_pixel_y = 7 - y;
            
            u8 top = *sprite_pixels++;
            u8 bot = *sprite_pixels++;
            
            for(int x = 0; x < 8; x++) {
                int sprite_pixel_x = x;
                if(mirror_x) sprite_pixel_x = 7 - x;
                
                if(!((top >> (7 - x)) & 1) && 
                   !((bot >> (7 - x)) & 1)) continue;
                
                int screen_pixel_x = sprite->x + sprite_pixel_x - 8;
                int screen_pixel_y = sprite->y + sprite_pixel_y - 16;
                
                if(screen_pixel_x < 0 || screen_pixel_x >= VIDEO_WIDTH ||
                   screen_pixel_y < 0 || screen_pixel_y >= VIDEO_HEIGHT) continue;
                
                grid->hidden_pixels[screen_pixel_x + screen_pixel_y * VIDEO_WIDTH] = true;
            }
        }
    }
    
}

void grid_hide_behind_menu_tiles(Grid *dest, u8 *vram, bool bank1, int offset_x, int offset_y) {
    u8 *tile_map = vram + (bank1 ? 0x1c00 : 0x1800);
    for(int tile_y = 0; tile_y < (VIDEO_HEIGHT - offset_y) / 8; tile_y++) {
        for(int tile_x = 0; tile_x < (VIDEO_WIDTH - offset_x) / 8; tile_x++) {
            if(tile_map[tile_x + tile_y * 32] >= 0x60) {
                for(int pixel_y = 0; pixel_y < 8; pixel_y++) {
                    for(int pixel_x = 0; pixel_x < 8; pixel_x++) {
                        int screen_x = tile_x * 8 + pixel_x + offset_x;
                        int screen_y = tile_y * 8 + pixel_y + offset_y;
                        dest->hidden_pixels[screen_x + screen_y * VIDEO_WIDTH] = true;
                    }
                }
            }
        }
    }
}

void grid_blittovideo(GameBoy *gb, int *dest, Grid *src) {
    memset(src->hidden_pixels, false, VIDEO_SIZE);
    
    gameboy_savestate(gb, gb->state_buffer);
    u8 *oam = gb->state_buffer + gb->state_offsets["hram"];
    u8 *vram = gb->state_buffer + gb->state_offsets["vram"];
    grid_hide_behind_sprites(src, oam, vram);
    
    u8 lcdc = gameboy_cpuread(gb, 0xff40);
    u8 window_x = gameboy_cpuread(gb, 0xff4b) - 7;
    u8 window_y = gameboy_cpuread(gb, 0xff4a);
    bool window_visible = lcdc & 0x20 && window_y < VIDEO_HEIGHT && window_x < VIDEO_WIDTH;
    
    if(window_visible) {
        grid_hide_behind_menu_tiles(src, vram, lcdc & 0x40, window_x, window_y);
    } else if(gb->game & GSC) {
        grid_hide_behind_menu_tiles(src, vram, lcdc & 0x8, 0, 0);
    }
    
    int *dest_row = dest;
    int *src_row = src->pixels + src->viewport_x + src->viewport_y * src->width;
    int pixel_buffer[4];
    
    for(int y = 0; y < VIDEO_HEIGHT; y++) {
        int *dest_ptr = dest_row;
        int *src_ptr = src_row;
        
        for(int x = 0; x < VIDEO_WIDTH; x += 4) {
            alphablend(pixel_buffer, dest_ptr, src_ptr);
            
            for(int i = 0; i < 4; i++) {
                if(!src->hidden_pixels[x + i + y * VIDEO_WIDTH]) {
                    dest_ptr[i] = pixel_buffer[i];
                }
            }
            
            dest_ptr+=4;
            src_ptr+=4;
        }
        
        dest_row += VIDEO_WIDTH;
        src_row += src->width;
    }
}

// Hides the grid overlay.
DLLEXPORT void gameboy_hidegrid(GameBoy *gb) {
    if(!gb->show_grid) return;
    
    gb->show_grid = false;
    gameboy_removebreakpoint(gb, gb->gamedata.battle_start_address);
    grid_free(&gb->grid);
}

// Displays a grid overlay showing the path that the player is taking, useful for the
// creation of videos displaying a manip path. For this functionality to work properly,
// you must use gameboy_executepath for overworld movement.
DLLEXPORT void gameboy_showgrid(GameBoy *gb) {
    if(gb->show_grid) return;
    
    gb->show_grid = true;
    gameboy_setbreakpoint(gb, gb->gamedata.battle_start_address, &gameboy_hidegrid);
}