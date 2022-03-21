#include "windows.h"

typedef void (*EventCallback)(MSG *msg);

static bool window_should_close = false;

struct Window {
    HWND hwnd;
    int width;
    int height;
    DWORD style;
    EventCallback event_callback;
    BITMAPINFO pixel_format;
    LARGE_INTEGER frequency;
    HMENU root_menu;
    int num_menus;
};

static LRESULT CALLBACK window_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
    LRESULT Result = 0;
    
    switch(message) {
        case WM_COMMAND: {
            
        } break;
        case WM_CLOSE:
        case WM_DESTROY: {
            window_should_close = true;
            PostQuitMessage(0);
        } break;
        
        default: {
            Result = DefWindowProc(window, message, wparam, lparam);
        } break;
    }
    
    return Result;
}

Window open_window(int width, int height, EventCallback event_callback) {
    Window result = {};
    result.width = width;
    result.height = height;
    result.event_callback = event_callback;
    
    WNDCLASSEXA window_class = {
        .cbSize = sizeof(window_class),
        .style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW,
        .lpfnWndProc = &window_proc,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = GetModuleHandle(NULL),
        .hIcon = 0,
        .hCursor = 0,
        .hbrBackground = 0,
        .lpszMenuName = 0,
        .lpszClassName = "grid-windowclass",
    };
    
    if(RegisterClassExA(&window_class)) {
        result.style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
        RECT window_rect = { 0, 0, width, height };
        AdjustWindowRect(&window_rect, result.style, false);
        
        result.hwnd = CreateWindowExA(0, window_class.lpszClassName, 
                                      "", result.style, 
                                      CW_USEDEFAULT, CW_USEDEFAULT,
                                      window_rect.right - window_rect.left,
                                      window_rect.bottom - window_rect.top,
                                      0, 0, window_class.hInstance, 0);
        
        result.pixel_format.bmiHeader.biSize = sizeof(result.pixel_format.bmiHeader);
        result.pixel_format.bmiHeader.biPlanes = 1;
        result.pixel_format.bmiHeader.biBitCount = 32;
        result.pixel_format.bmiHeader.biCompression = BI_RGB;
        
        QueryPerformanceFrequency(&result.frequency);
        
        SetFocus(result.hwnd);
    }
    
    return result;
}

bool poll_window_events(Window *window) {
    MSG message;
    while(PeekMessageA(&message, window->hwnd, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        window->event_callback(&message);
        DispatchMessageA(&message);
    }
    
    return window_should_close;
}

void set_window_title(Window *window, const char *title) {
    SetWindowTextA(window->hwnd, title);
}

void blit_to_window(Window *window, int *pixels, int blit_width, int blit_height) {
    window->pixel_format.bmiHeader.biWidth = blit_width;
    window->pixel_format.bmiHeader.biHeight = -blit_height;
    
    HDC dc = GetDC(window->hwnd);
    StretchDIBits(dc, 
                  0, 0, window->width, window->height,
                  0, 0, blit_width, blit_height,
                  pixels, &window->pixel_format,
                  DIB_RGB_COLORS, SRCCOPY);
    ReleaseDC(window->hwnd, dc);
}

r64 get_time(Window *window) {
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return (r64) counter.QuadPart / (r64) window->frequency.QuadPart;
}

HMENU add_menu(Window *window, HMENU parent, const char *name, UINT flags) {
    if(!window->root_menu) {
        window->root_menu = CreateMenu();
        SetMenu(window->hwnd, window->root_menu);
        
        RECT window_rect = { 0, 0, window->width, window->height };
        AdjustWindowRect(&window_rect, window->style, true);
        SetWindowPos(window->hwnd, NULL, 0, 0, 
                     window_rect.right - window_rect.left,
                     window_rect.bottom - window_rect.top,
                     SWP_NOMOVE);
    }
    
    if(parent == 0) parent = window->root_menu;
    
    HMENU menu = CreateMenu();
    AppendMenuA(parent, flags | MF_POPUP, (UINT_PTR) menu, name);
    DrawMenuBar(window->hwnd);
    return menu;
}

int add_menu_item(Window *window, HMENU parent, const char *name, UINT flags) {
    int id = window->num_menus++;
    AppendMenuA(parent, flags | MF_STRING, (UINT_PTR) id, name);
    DrawMenuBar(window->hwnd);
    return id;
}