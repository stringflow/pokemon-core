// Controls:
//   Left/Right arrow keys - Control time (hold ALT for fast forward)
//   Up/Down arrow keys    - Change view mode
//   A/D                   - Fine time control (only advances one frame per press)
//   Space                 - Play/Pause (toggle)
//   Home                  - Jump to start of the video
//   End                   - Jump to end of the video
//
// NOTE(stringflow): Video frames are stored in the dumbest way possible and uncompressed.
// Long manips may eat up 2-3gb of RAM.

#include <vector>
#include <thread>
#include "..\src\gameboy.cpp"
#include "window.cpp"
#include "manips.h"

enum ViewMode {
    VM_PIXELS,
    VM_HIDDEN_PIXELS,
    VM_GRID_PIXELS,
    VM_SIZE
};

struct VideoFrame {
    int **pixels;
};

struct GlobalState {
    Window *window;
    Manip *manip;
    std::vector<VideoFrame> frames;
    int current_frame;
    int view_mode;
    bool prerendering;
    bool playing;
};

static GlobalState *state;

void on_videoframe(GameBoy *gb, int *video_buffer) {
    gameboy_showgrid(gb);
    
    int **pixels = new int*[VM_SIZE];
    for(int i = 0; i < VM_SIZE; i++) {
        pixels[i] = new int[VIDEO_SIZE];
    }
    
    memcpy(pixels[VM_PIXELS], video_buffer, VIDEO_SIZE * VIDEO_BPP);
    
    if(gb->grid.hidden_pixels) {
        for(int i = 0; i < VIDEO_SIZE; i++) {
            pixels[VM_HIDDEN_PIXELS][i] = (gb->grid.hidden_pixels[i] ? 0xffffffff : 0x00000000);
        }
        
        for(int i = 0; i < VIDEO_SIZE; i++) {
            int grid_index = (gb->grid.viewport_x + gb->grid.viewport_y * gb->grid.width);
            grid_index += (i / VIDEO_WIDTH) * gb->grid.width;
            grid_index += i % VIDEO_WIDTH;
            int pixel = gb->grid.pixels[grid_index];
            pixels[VM_GRID_PIXELS][i] = pixel;
        }
    } else {
        memset(pixels[VM_HIDDEN_PIXELS], 0, VIDEO_SIZE * VIDEO_BPP);
        memset(pixels[VM_GRID_PIXELS], 0, VIDEO_SIZE * VIDEO_BPP);
    }
    
    VideoFrame frame = {
        .pixels = pixels,
    };
    
    state->frames.push_back(frame);
}

void free_video() {
    for(int i = 0; i < state->frames.size(); i++) {
        for(int j = 0; j < VM_SIZE; j++) {
            delete[] state->frames[i].pixels[j];
        }
        delete[] state->frames[i].pixels;
    }
    
    state->frames.clear();
}

void prerender_video(Manip *manip) {
    set_window_title(state->window, "PRE-RENDERING...");
    free_video();
    
    state->current_frame = 0;
    state->playing = false;
    state->view_mode = VM_PIXELS;
    state->manip = manip;
    
    GameBoy *gb = gameboy_create();
    gameboy_load(gb, "test\\roms\\gbc_bios.bin", manip->rom, manip->sav);
    
    gameboy_setvideocallback(gb, &on_videoframe);
    manip->procedure(gb, manip->path);
    gameboy_free(gb);
    state->prerendering = false;
}

void on_event(MSG *msg) {
    switch(msg->message) {
        case WM_COMMAND: {
            if(state->prerendering) return;
            
            state->prerendering = true;
            std::thread prerender_thread(prerender_video, &all_manips[(int) msg->wParam]);
            prerender_thread.detach();
        } break;
        case WM_SYSKEYUP:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_KEYDOWN: {
            if(state->prerendering) return;
            
            u32 keycode = msg->wParam;
            bool alt = msg->lParam & (1 << 29);
            bool was_down = msg->lParam & (1 << 30);
            
            int time = 1;
            if(alt) time = 10;
            
            if(keycode == VK_RIGHT || (keycode == 'D' && !was_down)) {
                state->current_frame += time;
            } else if(keycode == VK_LEFT || (keycode == 'A' && !was_down)) {
                state->current_frame -= time;
            } else if(keycode == VK_UP && !was_down) {
                state->view_mode++;
            } else if(keycode == VK_DOWN && !was_down) {
                state->view_mode--;
            } else if(keycode == VK_END && !was_down) {
                state->current_frame = 2147483647;
            } else if(keycode == VK_HOME && !was_down) {
                state->current_frame = 0;
            } else if(keycode == VK_SPACE && !was_down) {
                state->playing = !state->playing;
            }
        } break;
    }
}

int main(int argc, char *argv[]) {
    state = new GlobalState();
    
    int scale = 3;
    Window window = open_window(VIDEO_WIDTH*scale, VIDEO_HEIGHT*scale, &on_event);
    state->window = &window;
    
    HMENU manip_menu = add_menu(&window, 0, "Manips", 0);
    HMENU game_menu;
    char *current_game;
    for(int i = 0; i < ARRAY_SIZE(all_manips); i++) {
        Manip *manip = &all_manips[i];
        if(!current_game || strcmp(current_game, manip->game) != 0) {
            game_menu = add_menu(&window, manip_menu, manip->game, 0);
            current_game = manip->game;
        }
        
        add_menu_item(&window, game_menu, manip->name, 0);
    }
    
    r64 seconds_per_frame = 1.0 / 60.0;
    r64 last_time = get_time(&window);
    r64 delta = 0;
    
    while(!poll_window_events(&window)) {
        r64 current_time = get_time(&window);
        delta += (current_time - last_time) / seconds_per_frame;
        last_time = current_time;
        if(delta >= 1) {
            if(state->playing) state->current_frame++;
            delta--;
        }
        
        state->current_frame = CLAMP(state->current_frame, 0, state->frames.size()-1);
        state->view_mode = CLAMP(state->view_mode, 0, VM_SIZE-1);
        
        if(!state->prerendering && state->frames.size() > 0) {
            char title[64];
            sprintf(title, "%s | %s (%d/%d)", state->manip->name, state->playing ? "PLAYING" : "PAUSED", (int) state->current_frame, (int) state->frames.size()-1);
            set_window_title(state->window, title);
            
            VideoFrame frame = state->frames[state->current_frame];
            blit_to_window(&window, 
                           frame.pixels[state->view_mode],
                           VIDEO_WIDTH, VIDEO_HEIGHT);
        }
        Sleep(1);
    }
    
    free_video();
    delete state;
    
    DestroyWindow(window.hwnd);
}