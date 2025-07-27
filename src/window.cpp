#include "window.hpp"

#include "config.hpp"
#include "layout.hpp"
#include "main.hpp"
#include <raylib.h>

// Function Declarations
void WindowThread();
void DrawFrame();

// Function Definitions
void WindowThread() {
    const int width = KBMVConfig.window.width,
              height = KBMVConfig.window.height;

    SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(width, height, "kbmv");

    SetWindowMinSize(width, height);
    SetWindowMaxSize(width, height);

    SetTargetFPS(60);

    LayoutMutex.lock();
    for (LayoutItemBase *item : KBMVLayout.items) {
        item->update_position();
    }
    LayoutMutex.unlock();

    while (!WindowShouldClose()) {
        if (!IsRunning.load()) {
            CloseWindow();
            break;
        }
        DrawFrame();
    }
}

void DrawFrame() {
    BeginDrawing();

    ClearBackground(BLANK);

    if (KBMVConfig.window.show_fps)
        DrawFPS(8, 8);

    LayoutMutex.lock();
    for (LayoutItemBase *item : KBMVLayout.items) {
        item->draw();
    }
    LayoutMutex.unlock();

    EndDrawing();
}
