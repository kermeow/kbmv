#include "window.hpp"

#include "raylib.h"

void WindowThread() {
    int width = 800, height = 600;
    InitWindow(width, height, "kbmv");

    SetWindowMinSize(width, height);
    SetWindowMaxSize(width, height);

    SetTargetFPS(30);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Color{.a = 0});
        DrawFPS(8, 8);
        EndDrawing();
    }
}