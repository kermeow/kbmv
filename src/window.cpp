#include "window.hpp"

#include "config.hpp"
#include "main.hpp"
#include <raylib.h>

// Function Declarations
void WindowThread();
void DrawFrame();

// Function Definitions
void WindowThread() {
    const int width = KBMVConfig.window.width,
              height = KBMVConfig.window.height;

    SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_UNDECORATED);
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(width, height, "kbmv");

    SetWindowMinSize(width, height);
    SetWindowMaxSize(width, height);

    SetTargetFPS(60);

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

    DrawFPS(8, 8);

    EndDrawing();
}